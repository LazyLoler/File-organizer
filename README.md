File Organizer

This is a lightweight C utility that organizes files based on their actual content using magic byte detection. It scans files in a specified folder, identifies their types, and moves them into corresponding subfolders.

Features
- Detects file types using magic bytes (not just extensions)
- Supports common formats like PDF, MP3, PNG, DOCX, ZIP, and more
- Automatically creates folders and moves files
- Logs all actions to a log file
- Modular and extensible codebase
  
Usage
- Place sorter.exe in any folder
- Create a subfolder named Entry in the same directory
- Add files to the Entry folder
- Run sorter.exe
- Files will be sorted into folders by type
