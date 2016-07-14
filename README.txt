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

TODO:
 - refactor code to make it cleaner
   - get rid of uncalled function and superfluous includes
   - rewrite code to get the info first, then print it. It will be easier this way.
   - get rid of those stupid read_(four|eight)_bytes() functions.
 - parse section C
 - parse section D
 - parse sub-section E
 - parse sub-section F
 - convert all digit value to hex
 - dump unknown values anyway, as hex bytes à la hexdump
 - make sure to test every prefetch version
   - Write a win10 prefetch decompresser
 - Make sure to double check the results, and maybe ask for people that knows best.
 - Write on this thing to make it stick.
