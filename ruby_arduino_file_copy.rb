#Ruby_arduino_file_copy.rb by Vincent Gosselin, copyright 2016.

#This simple script is made to get ride of the anoying Copy-Paste
#from sublime text into arduino IDE and texts editors to program the arduino.

#This is a script that copies sublime text files
#Arduino file (.ino), .cpp file and .h file into the respective 
#files.


#Source
growbox1_arduinodue_cpp = File.readlines("/Users/vincentgosselin/Desktop/growbox_technologies/Growbox1_software/Growbox1/growbox1_arduinodue.cpp")
library_cpp = File.readlines("/Users/vincentgosselin/Desktop/growbox_technologies/Growbox1_software/Growbox1/GROWBOX1.cpp")
library_h = File.readlines("/Users/vincentgosselin/Desktop/growbox_technologies/Growbox1_software/Growbox1/GROWBOX1.h")

#Destination files
path_to_growbox1_arduinodue_ino_file = "/Users/vincentgosselin/Documents/Arduino/growbox1_arduinodue/growbox1_arduinodue.ino"
path_to_library_cpp_file = "/Users/vincentgosselin/Documents/Arduino/libraries/GROWBOX1/GROWBOX1.cpp"
path_to_library_h_file = "/Users/vincentgosselin/Documents/Arduino/libraries/GROWBOX1/GROWBOX1.h"

#COPY-PASTE
File.open(path_to_growbox1_arduinodue_ino_file, "w") do |f|
	f.puts growbox1_arduinodue_cpp
end

File.open(path_to_library_cpp_file, "w") do |f|
	f.puts library_cpp
end

File.open(path_to_library_h_file, "w") do |f|
	f.puts library_h
end

puts "COPY-PASTE DONE"



