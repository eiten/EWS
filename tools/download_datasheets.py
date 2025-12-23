import csv
import os
import re
import time
import hashlib
import requests
from bs4 import BeautifulSoup

# --- Configuration ---
BOM_PATH = 'production/bom.csv'
OUTPUT_DIR = 'doc/datasheets'
PRODUCT_PAGE_URL = 'https://www.lcsc.com/product-detail/'
DATASHEET_BASE_URL = 'https://wmsc.lcsc.com/wmsc/upload/file/pdf/v2/'
HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
}

# --- Helper Functions ---
def sanitize_filename(name):
    """Replaces characters that are invalid for filenames."""
    return re.sub(r'[\\/*?:\"<>|]', '_', name)

def get_mfr_part_number(session, lcsc_part_number):
    """Fetches the product page and parses it for the Mfr. Part #."""
    product_url = f"{PRODUCT_PAGE_URL}{lcsc_part_number}.html"
    try:
        response = session.get(product_url, timeout=15)
        response.raise_for_status()
        soup = BeautifulSoup(response.text, 'html.parser')
        
        mfr_title_cell = soup.find(lambda tag: tag.name == 'td' and 'Mfr. Part #' in tag.get_text(strip=True))
        if mfr_title_cell:
            mfr_value_cell = mfr_title_cell.find_next_sibling('td')
            if mfr_value_cell and mfr_value_cell.find('span'):
                return mfr_value_cell.find('span').text.strip()
    except requests.exceptions.RequestException as e:
        print(f"  - ERROR: Could not fetch product page for {lcsc_part_number}. {e}")
    except Exception as e:
        print(f"  - ERROR: Parsing Mfr. Part # failed for {lcsc_part_number}. {e}")
    return None

def download_pdf(session, lcsc_part, mfr_part):
    """Downloads a PDF and saves it with the Mfr. Part #."""
    pdf_url = f"{DATASHEET_BASE_URL}{lcsc_part}.pdf"
    sanitized_mfr_part = sanitize_filename(mfr_part)
    filepath = os.path.join(OUTPUT_DIR, f"{sanitized_mfr_part}.pdf")

    if os.path.exists(filepath):
        print(f"  - INFO: File already exists for Mfr. Part '{mfr_part}'. Skipping.")
        return filepath, True # Return path for deduplication check

    print(f"  - Downloading: {pdf_url}")
    try:
        pdf_response = session.get(pdf_url, timeout=60)
        pdf_response.raise_for_status()
        
        if 'application/pdf' in pdf_response.headers.get('Content-Type', ''):
            with open(filepath, 'wb') as f:
                f.write(pdf_response.content)
            print(f"  - SUCCESS: Saved to {filepath}")
            return filepath, True
        else:
            print(f"  - ERROR: URL did not return a PDF. Content-Type: {pdf_response.headers.get('Content-Type', 'N/A')}")
            return None, False
    except requests.exceptions.RequestException as e:
        print(f"  - ERROR: Could not download PDF for {lcsc_part}. {e}")
        return None, False

def deduplicate_files():
    """Finds duplicate files in the output directory and replaces them with symlinks."""
    print("\n--- Starting Deduplication Process ---")
    hashes = {}
    files_to_process = [f for f in os.listdir(OUTPUT_DIR) if f.endswith('.pdf')]

    for filename in files_to_process:
        filepath = os.path.join(OUTPUT_DIR, filename)
        if os.path.islink(filepath):
            continue

        with open(filepath, 'rb') as f:
            file_hash = hashlib.sha256(f.read()).hexdigest()

        if file_hash in hashes:
            original_file = hashes[file_hash]
            print(f"  - DUPLICATE: '{filename}' is identical to '{os.path.basename(original_file)}'.")
            os.remove(filepath)
            # Create a relative symlink
            relative_path = os.path.relpath(original_file, start=OUTPUT_DIR)
            os.symlink(relative_path, filepath)
            print(f"  - ACTION: Replaced '{filename}' with a symbolic link.")
        else:
            hashes[file_hash] = filepath
            print(f"  - UNIQUE: '{filename}'")
    print("--- Deduplication Finished ---")

# --- Main Execution ---
def main():
    """Main script execution."""
    if not os.path.exists(BOM_PATH):
        print(f"FATAL: BOM file not found at '{BOM_PATH}'")
        return

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    # --- Phase 1: Gather Info & Download ---
    print("--- Starting Download Process ---")
    download_tasks = {} # {lcsc_part: mfr_part}
    
    with open(BOM_PATH, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        unique_lcsc_parts = {row.get('LCSC Part #', '').strip() for row in reader if row.get('LCSC Part #', '').strip()}

    with requests.Session() as session:
        session.headers.update(HEADERS)
        
        for i, lcsc_part in enumerate(unique_lcsc_parts):
            print(f"\nProcessing LCSC Part {i+1}/{len(unique_lcsc_parts)}: {lcsc_part}")
            mfr_part = get_mfr_part_number(session, lcsc_part)
            if mfr_part:
                download_tasks[lcsc_part] = mfr_part
            time.sleep(0.5) # Be a good citizen

        print("\n--- All product information gathered. Starting downloads. ---")
        for lcsc_part, mfr_part in download_tasks.items():
            print(f"\nDownloading for LCSC Part: {lcsc_part} (Mfr: {mfr_part})")
            download_pdf(session, lcsc_part, mfr_part)
            time.sleep(0.5)

    # --- Phase 2: Deduplicate ---
    deduplicate_files()
    
    print("\nScript finished.")

if __name__ == '__main__':
    main()
