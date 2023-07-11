# PersistenceManager Library for Arduino ESP32

The PersistenceManager is a library for ESP32 devices that allows the persistence of key-value pairs on the device's LittleFS file system. The library uses JSON for managing and storing these key-value pairs.

## Features

- CRUD operations: Create, Read, Update and Delete key-value pairs.
- Atomic transactions: Ensures data integrity during power loss or system crashes.
- Supports basic data types, arrays and nested objects.
- Thread safe: The library is suitable for use in multi-threaded applications.
- Backup: Ability to create a backup of the data file.
- Error handling: Clear error messages for easier debugging.
- Checks for available space: The library ensures that there is enough space on the file system before writing to the file.

## Usage

```cpp
#include "PersistenceManager.h"

PersistenceManager pm("/data.json");

// Set a key-value pair.
pm.set("key", "value");

// Get the value of a key.
String value = pm.get("key");

// Remove a key-value pair.
pm.remove("key");

// Check if a key exists.
bool exists = pm.exists("key");

// List all keys.
String keys = pm.listKeys();

// Save the JSON document to the file.
pm.save();

// Clear all key-value pairs.
pm.clear();

// Create a backup of the data file.
pm.backup("/backup.json");
```

## Installation

1. Download the repository as a ZIP file.
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library...
3. Select the downloaded ZIP file.
4. Now you can include the PersistenceManager library in your sketch by going to Sketch > Include Library > PersistenceManager.

## Notes

- The default size of the JSON document is 2048 bytes. If you need to store larger documents, you will need to increase this value.

```
PersistenceManager manager("/data.json", 4096);
```

- The library does not automatically save the document to the file after each operation. You need to call the `save` method when you want to persist the changes.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

---

Please note, you'll need to include a `LICENSE` file in your project repository with the full text of the GNU General Public License v3.0.