import sys

def read_hex_dump(file):
    print("const unsigned char RAM[] = {")
    
    # Parse each line of the hex dump
    for line in file:
        # Skip empty lines
        if not line.strip():
            continue
            
        # Remove ASCII representation part if present
        if '|' in line:
            line = line.split('|')[0].strip()
            
        # Split the line into address and data
        parts = line.strip().split()
        if len(parts) < 2:
            continue
            
        hex_data = parts[1:]
        
        # Convert hex strings to bytes
        try:
            bytes_data = bytes.fromhex(''.join(hex_data))
            # Format as C hex values
            hex_values = [f"0x{byte:02x}" for byte in bytes_data]
            print("    " + ", ".join(hex_values) + ",")
        except ValueError as e:
            print(f"Error parsing line: {line.strip()}")
            print(f"Error: {e}")
    
    print("};")
    print("const int RAMLEN = sizeof(RAM);")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        # Read from file specified as command line argument
        with open(sys.argv[1], 'r') as file:
            read_hex_dump(file)
    else:
        # Read from stdin
        read_hex_dump(sys.stdin) 