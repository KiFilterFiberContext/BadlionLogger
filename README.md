# BadlionLogger
BadlionLogger is a rudimentary PoC of a kernel driver used to monitor the activity of BadlionClient's kernelmode anti-cheat (BadlionAnticheat.sys) by applying IAT hooks on driver load.

# Examples
## DriverEntry
<img src="https://i.imgur.com/W1O7wPR.png">

## CreateProcess callback
<img src="https://i.imgur.com/WoJkeg1.png">

# Note
BadlionLogger is a proof-of-concept and is not fully complete.
Several possible upcoming improvements include:
- Using MDLs instead of flipping CR0.WP
- Hooking dynamically resolved imports 
- MajorFunction pointer hooks