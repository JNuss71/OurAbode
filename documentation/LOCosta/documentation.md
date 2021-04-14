# File Read and Write from Arduino

- The brackets in the below are just spaces for arguments, you ***DO NOT*** need to include the brackets when calling the below functions/methods.
- To write from a file, one must first call `Serial.println("SETWFILE:[FILE_NAME]")` in order to set the destination. Avoid spaces in file names if possible, also do not include the extension (files are .txt by default).
- Once the destination is set, one must call `Serial.println("WFILE:[TAG_NAME]=[VALUE]")`. `[TAG_NAME]` is what you use to read from the file.
- For example, if I wanted to store a variable for the intensity of the red LED for `somePerson`, I might do the following:
```
Serial.println("SETWFILE:somePersonPrefs");
Serial.println("WFILE:led_red_strength=" + 2);
```

- In order to read from a file, the syntax is a bit different. You instead call: `Serial.println("RFILE:[FILE_NAME] [TAG_NAME]")`. Once again you do not include the file extension.
- Once again, using the example from above, if I wanted to get the stored value for the intensity of the red LED for `somePerson`, I would do the following:
```
Serial.println("RFILE:somePersonPrefs led_red_strength");
```
- You can then retrieve this information via `Serial.readString()`. As you might guess, this information is transmitted as a String.

- If any string printed to `Serial` begins with a semicolon (;), it will not be parsed for file I/O, and will just be displayed to the console, semicolon does get stripped.
