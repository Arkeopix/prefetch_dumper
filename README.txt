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
 - File metrics: i'm not quite sure about those values
 - Trace chains: i'm not quite sure about those values

TODO:
 - parse section C
 - parse section D
 - parse sub-section E
 - parse sub-section F
 - refactor code to make it cleaner
   - Make sure to be consitent with utility function: union or pointer tricks ?
   - get rid of those stupid read_(four|eight)_bytes() functions.
   - maybe create a struct reader to handle all the reads instead of relying on offset and lseeks
 - make sure to test every prefetch version
   - Write a win10 prefetch decompresser
 - Make sure to double check the results, and maybe ask for people that knows best.
 - Write on this thing to make it stick.
