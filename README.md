# IsDebuggerPresent-bypass
Quick and dirty way to bypass the IsDebuggerPresent function

---
# Project Structure

## Hooker
  The actual code to bypass IsDebuggerPresent
 
## IsDebuggerPresentHooking
  A *very* small program that always print the "state" of IsDebuggerPresent
  

# How does Hooker works ?
It's a simple DLL to inject that hooks the IsDebuggerPresent function and make it always return 0

I might not touch this small program again. Feel free to fork it and improve it
