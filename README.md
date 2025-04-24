# Qt Notepad Project

## Overview
This project is a simple text editor built using Qt, designed to provide basic text editing functionalities along with some optional extended features. It serves as a practical example of using Qt for GUI application development.

## Features
### Core Features
- **Text Editing**: Supports multi-line text input, copy/paste/cut, and select all.
- **File Operations**: Create, open, save, and save as (supports .txt format).
- **Formatting Options**: Allows selection of font type, size, and color.
- **Find and Replace**: Supports case-sensitive and whole word matching.
- **Status Display**: Shows current cursor position (line number/column number) and file encoding format.
- **Basic Interactions**: Undo/Redo functionality and an About dialog.

### Extended Features (Optional)
- **Multi-Tab Editing**: Allows editing of multiple documents in tabs.
- **Syntax Highlighting**: Supports basic syntax highlighting for programming languages.
- **Auto Save and Recovery**: Automatically saves documents and recovers unsaved changes.
- **Customizable Shortcuts**: Users can define their own keyboard shortcuts.

## Technical Architecture
- **Architecture Pattern**: 
  - MainWindow (A) connects to UI Module (B), Business Logic Module (C), and File Operations Module (D).
  - Business Logic Module (C) connects to Editing Functionality (E) and Find/Replace (F).
  - File Operations Module (D) connects to Encoding Conversion (G) and Exception Handling (H).

- **Technology Stack**: 
  - Qt 5.12+ (LGPL License)
  - C++11 Standard

## Getting Started
1. **Environment Setup**: 
   - Use Qt Creator to create a new project.
   - Select QMainWindow as the base class.

2. **Building the Project**: 
   - Open the project file `notepad.pro` in Qt Creator.
   - Build the project to compile the source files.

3. **Running the Application**: 
   - After building, run the application from Qt Creator.

## Testing
- Unit tests are included to verify core functionalities such as file saving and text editing.
- Automated UI tests can be implemented using the Qt Test module.

## Future Enhancements
- Consider implementing a plugin system for additional functionality.
- Integrate version control features for better file management.
- Explore AI integration for intelligent text completion.

## Resources
- [Qt Official Documentation](https://doc.qt.io)
- Example project templates can be found on GitHub by searching for "qt-notepad-example".

## License
This project is licensed under the terms of the LGPL.
