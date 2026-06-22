import sys
import os

def png_to_ico_standard(png_path, ico_path=None):
    if not os.path.exists(png_path):
        print(f"Error: File '{png_path}' not found.")
        return False
        
    if not ico_path:
        ico_path = os.path.splitext(png_path)[0] + ".ico"
        
    with open(png_path, "rb") as f:
        png_data = f.read()
        
    # Check if it is a valid PNG
    if not png_data.startswith(b"\x89PNG\r\n\x1a\n"):
        print("Error: Input file is not a valid PNG.")
        return False

    png_size = len(png_data)
    
    # Extract width and height from PNG IHDR chunk (bytes 16-23)
    width = int.from_bytes(png_data[16:20], byteorder="big")
    height = int.from_bytes(png_data[20:24], byteorder="big")
    
    # In ICO directory, width and height of 256 are represented as 0
    ico_w = 0 if width >= 256 else width
    ico_h = 0 if height >= 256 else height
    
    # Create the 22-byte ICO header
    # 1. ICONHEADER (6 bytes)
    # - Reserved: 2 bytes (0)
    # - Type: 2 bytes (1 for icon)
    # - Count: 2 bytes (1 image)
    icon_header = b"\x00\x00\x01\x00\x01\x00"
    
    # 2. ICONDIRENTRY (16 bytes)
    # - Width: 1 byte
    # - Height: 1 byte
    # - ColorCount: 1 byte (0)
    # - Reserved: 1 byte (0)
    # - Planes: 2 bytes (1)
    # - BPP: 2 bytes (32)
    # - BytesInRes: 4 bytes (size of PNG data)
    # - ImageOffset: 4 bytes (22)
    icon_dir_entry = bytes([
        ico_w,
        ico_h,
        0, # Color count
        0, # Reserved
    ])
    icon_dir_entry += (1).to_bytes(2, byteorder="little")      # Planes
    icon_dir_entry += (32).to_bytes(2, byteorder="little")     # BPP
    icon_dir_entry += png_size.to_bytes(4, byteorder="little") # Size of PNG
    icon_dir_entry += (22).to_bytes(4, byteorder="little")     # Offset (22 bytes)
    
    # Write the ICO file
    with open(ico_path, "wb") as f_out:
        f_out.write(icon_header)
        f_out.write(icon_dir_entry)
        f_out.write(png_data)
        
    print(f"Success! Converted '{png_path}' ({width}x{height}) to '{ico_path}'")
    return True

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python png_to_ico.py <input.png> [output.ico]")
    else:
        png_to_ico_standard(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else None)
