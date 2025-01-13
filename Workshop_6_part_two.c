import os
import shutil
import json

def create_file(file_name, file_type, content=""):
    """Creates a file of the specified type and writes the initial content to it."""
    try:
        if file_type == '.txt':
            with open(file_name, 'w') as f:
                f.write(content)
        elif file_type == '.bin':
            with open(file_name, 'wb') as f:
                f.write(content.encode())  # Encoding string content to binary
        elif file_type == '.csv':
            with open(file_name, 'w', newline='') as f:
                f.write(content)  # CSV content as string
        elif file_type == '.json':
            with open(file_name, 'w') as f:
                json.dump(content, f)  # Assuming content is a dict or list
        else:
            print(f"Unsupported file type: {file_type}")
    except Exception as e:
        print(f"Error creating file: {e}")

def append_to_file(file_name, content):
    """Appends content to an existing file."""
    try:
        with open(file_name, 'a') as f:
            f.write(content)
    except Exception as e:
        print(f"Error appending to file: {e}")

def copy_file(source_file, destination_file):
    """Copies an existing file to a new location."""
    try:
        shutil.copy(source_file, destination_file)
    except Exception as e:
        print(f"Error copying file: {e}")

def delete_file(file_name):
    """Deletes the specified file."""
    try:
        if os.path.exists(file_name):
            os.remove(file_name)
            print(f"File {file_name} deleted successfully.")
        else:
            print(f"File {file_name} does not exist.")
    except Exception as e:
        print(f"Error deleting file: {e}")

def read_file(file_name):
    """Reads and displays the content of the specified file."""
    try:
        if file_name.endswith('.txt'):
            with open(file_name, 'r') as f:
                return f.read()
        elif file_name.endswith('.bin'):
            with open(file_name, 'rb') as f:
                return f.read().decode()  # Decoding binary content to string
        elif file_name.endswith('.csv'):
            with open(file_name, 'r') as f:
                return f.read()
        elif file_name.endswith('.json'):
            with open(file_name, 'r') as f:
                return json.load(f)
        else:
            print("Unsupported file type.")
            return None
    except Exception as e:
        print(f"Error reading file: {e}")
        return None

# Example Usage:

# Create a .txt file and add some content
create_file('sample.txt', '.txt', "Hello, this is a sample text file.")

# Append content to the .txt file
append_to_file('sample.txt', "\nThis is an appended line.")

# Create a .json file with dictionary content
create_file('data.json', '.json', {"name": "Alice", "age": 30})

# Copy a file
copy_file('sample.txt', 'sample_copy.txt')

# Read a .txt file
print(read_file('sample.txt'))

# Delete a file
delete_file('sample_copy.txt')