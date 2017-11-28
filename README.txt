Prefetch Dumper

This is a command line tool allowing one to dump the content of windows prefetch files on linux.
This is work in progress, however, the forensic value of the software is still good enough.
So far, one can see the following:
 - Signature
 - File length
 - Executable name
 - Path hash
 - Last execution
 - Run count
 - File metrics
 - Trace chains
 - Filename strings
 - Volume information
 - NTFS File references
 - Directory strings

TODO:
 - refactor code to make it cleaner
   - get rid of uncalled function and superfluous includes
   - maybe, just maybe, consider macro to avoid repeting yourself
   - Check every return value
   - improve prefetch struct and stuff some function pointer in it
 - convert all digit value to hex
 - make sure to test every prefetch version
   - version 17, 26, 30
   - Write a win10 prefetch decompresser
 - Make sure to double check the results, and maybe ask for people that knows best.
 - Free all allocated memory
 - Write on this thing to make it stick.
