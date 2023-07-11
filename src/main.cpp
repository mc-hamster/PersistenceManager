#include <Arduino.h>
#include "PersistenceManager.h"

// You can specify the maximum JSON size when creating the instance.
// Here, we're specifying a maximum size of 4096 bytes.
PersistenceManager manager("/data.json", 4096);

void setup()
{
  Serial.begin(921600);
  delay(200); // Short delay to allow the serial port to initialize

  Serial.println("--------------------------");

  if (!LittleFS.begin())
  {
    Serial.println("An error occurred while mounting LittleFS, formatting...");
    LittleFS.format();
    if (!LittleFS.begin())
    {
      Serial.println("Formatting did not help. Cannot use LittleFS. Halting execution.");
      while (1)
        ;
    }
  }

  // Load the JSON document from the file.
  manager.load();

  // Set some key-value pairs.
  manager.set("key1", "value1");
  manager.set("key2", 42);

  // Create a nested JSON object.
  JsonObject obj = manager._doc.createNestedObject("nestedObject");
  obj["nestedKey1"] = "nestedValue1";
  obj["nestedKey2"] = 24;

  // Create an array and add values.
  JsonArray arr = manager._doc.createNestedArray("myArray");
  arr.add(1);
  arr.add(2);
  arr.add(3);

  // Save the document to the file.
  if (manager.save())
  {
    Serial.println("Data saved successfully.");
  }
  else
  {
    Serial.println("Failed to save data.");
  }

  // Check if a key exists.
  if (manager.exists("key1"))
  {
    Serial.println("key1 exists.");
  }
  else
  {
    Serial.println("key1 does not exist.");
  }

  // Print all keys.
  Serial.println("All keys:");
  Serial.println(manager.listKeys());

  // Get and print the size of the JSON file
  size_t fileSize = manager.getFileSize();
  Serial.print("Size of the JSON file: ");
  Serial.println(fileSize);

  // Backup the data to a different file.
  if (manager.backup("/backup.json"))
  {
    Serial.println("Data backed up successfully.");
  }
  else
  {
    Serial.println("Failed to backup data.");
  }
}

void loop()
{
  // Delay for a bit before reading the data back.
  delay(2000);

  // Get and print a value.
  Serial.println("key1: " + String(manager.get("key1").as<const char *>()));
  Serial.println("key2: " + String(manager.get("key2").as<int>()));

  // Get and print a nested JSON object.
  JsonObject obj = manager.get("nestedObject").as<JsonObject>();
  Serial.println("nestedKey1: " + String(obj["nestedKey1"].as<const char *>()));
  Serial.println("nestedKey2: " + String(obj["nestedKey2"].as<int>()));

  // Get and print an array.
  Serial.println("myArray:");
  JsonArray arr = manager.get("myArray").as<JsonArray>();
  for (JsonVariant v : arr)
  {
    Serial.println(v.as<int>());
  }

  // Remove a key-value pair and save the data.
  if (manager.remove("key1"))
  {
    Serial.println("Removed key1.");
  }
  else
  {
    Serial.println("Failed to remove key1.");
  }

  Serial.println(millis());

  manager.printFileContents();

  // Clear all data and save the changes.
  // manager.clear();
  // Serial.println("Cleared all data.");
}
