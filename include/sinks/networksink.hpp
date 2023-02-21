/*
[TODO] 
* This file allows us to write to remote disk or network locations.
* So the logic for this file is to create a thread which will be responsible for writing to the network location. 
* This allows us to decouple logging and uploading resulting in better performance for the application.
* This idea is similar to the lazy writer in the console sink.
*/