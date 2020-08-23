# BadlionLogger
BadlionLogger is a rudimentary PoC of a kernel driver used to monitor the activity of BadlionClient's kernelmode anti-cheat (BadlionAnticheat.sys) by applying IAT hooks on driver load.

# Examples
## DriverEntry

<img src="https://imgur.com/W1O7wPR"/>

## CreateProcess callback
<img src="https://imgur.com/WoJkeg1"/>

# Note
BadlionLogger is a proof-of-concept and is not fully complete.
Several possible upcoming improvements include:
- Using MDLs instead of flipping CR0.WP
- Hooking dynamically resolved imports 
- MajorFunction pointer hooks
