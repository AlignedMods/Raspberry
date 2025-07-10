# The Raspberry Level format (.rsp)

### **NOTE:** Throughout this file the format will be reffered to as "*RSP*"

The RSP format can be divided into 2 parts:

1. **The Header**
2. **The Body**

## The Header
The Header is there to provide some basic info about the level (such as the level's name)

The header **MUST** contain the following parts!

1. A 3 byte long string containing the text "*RSP*" (no null-termination character)
2. A 1 byte long **unsigned integer** (int8_t) containing the length of the name of the level
3. A string containing the name of the level (no null-termination character)
4. The string "*_H_E_*" (no null-termination character)

An example of what an RSP format header might look like with the name "Mountain" (hex)

	0x52 0x53 0x50                          -- The "RSP" string
	0x08                                    -- The length of the name
	0x4D 0x6F 0x75 0x6E 0x74 0x61 0x69 0x6E -- The name
	0x5F 0x48 0x5F 0x45 0x5F                -- "_H_E_"


