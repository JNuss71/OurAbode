setlocal
set PATH=%PATH%;C:\Program Files (x86)\Arduino\
"C:\Program Files\Java\jdk1.8.0_101\bin\javac" -cp "C:\Users\thieraufc\Documents\GitHub\HackUMass-Project\src\Java\libs\RXTXcomm.jar" SerialTest.java
"C:\Program Files\Java\jdk1.8.0_101\bin\java" -cp "C:\Users\thieraufc\Documents\GitHub\HackUMass-Project\src\Java\libs\RXTXcomm.jar;." SerialTest
pause