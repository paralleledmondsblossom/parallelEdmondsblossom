import os
import requests
import tarfile
from io import BytesIO

def download_and_extract(url):
    # Extracting file name and directory name from the URL
    file_name = url.split("/")[-1]
    directory_name = file_name.split(".")[0]
    mtx_file = file_name.replace('.tar.gz', '.mtx')
    print("Checking", os.path.join(directory_name, mtx_file))
    # Checking if the file already exists in the specified directory
    if os.path.exists(os.path.join(directory_name, mtx_file)):
        print(f"The file {file_name} already exists in the directory {directory_name}.")
        return

    # Downloading the tar.gz file
    response = requests.get(url)
    if response.status_code != 200:
        print(f"Failed to download the file from {url}. HTTP status code: {response.status_code}")
        return

    # Extracting the tar.gz file
    with tarfile.open(fileobj=BytesIO(response.content), mode="r:gz") as tar:
        tar.extractall()

    # Checking if the desired file is present in the extracted directory
    extracted_file_path = os.path.join(directory_name, mtx_file)
    if not os.path.exists(extracted_file_path):
        print(f"Error: The file ",mtx_file," not found in the extracted directory {directory_name}.")
        return

    print(f"The file {file_name} has been successfully downloaded and extracted to {directory_name}.")

# URL to download
url = "http://sparse-files.engr.tamu.edu/MM/DIMACS10/kron_g500-logn21.tar.gz"

# Call the function to download and extract
download_and_extract(url)