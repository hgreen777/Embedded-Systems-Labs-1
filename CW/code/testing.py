import serial
import time
import argparse
import sys
RED = "\033[31m"
GREEN = "\033[32m"
RESET = "\033[0m"
# Call this program with
# python coa202ser.py portdevice timeout
# e.g. on windows:
#      python coa202ser.py COM2
#
# Please note that if timeout (for serial) is omitted a default value of 5 (seconds) will be used
# Remember you cannot leave this python program running while uploading your compiled Arduino sketch to the Arduino
#

# The code below parses the command line arguments and leaves the
# variable args.port containing the device name.

parser = argparse.ArgumentParser(
    description='Talk to Ardunio over serial')
parser.add_argument(
    'port',
    metavar='device',
    type=str,
    nargs='?',
    help='The port to connect to')
parser.add_argument(
    'timeout',
    metavar='timeout',
    type=int,
    nargs='?',
    default=5,
    help='The timeout for the serial message')
args = parser.parse_args()

# Open the serial port (this will reset your Arduino)
print(RESET + 'connecting to port', args.port)
ser = serial.Serial(args.port, 9600, timeout=args.timeout,
                    rtscts=False, xonxoff=False, dsrdtr=False)

print("waiting for sync")
going = True
while going:
    s = ser.read(1)  # Read just one byte
    # print(s)       # Print it for debugging
    if s == b'R':
        going = False
print("Found R")
#ser.write(b'BEGIN\n')
#line = ser.readline()
#print(line)
ser.write(b'BEGIN')
print("Sync")

line = ser.readline()
print(line)  # This should print BASIC or your extension list


# Build a list of messages to send
# the b'' notation creates byte arrays suitable for
# passing to ser.write().  ser.write() will not accept
# str variables.

# Include add for node with same id
general = [[b'Aaeaf-gd-fg-xvcsd',"INVALID"]]

add =  [[b'ADD-1000001-1-TO_EDIT_DEL',          "Valid"],
        [b'ADD-',                             "Invalid"],
        [b'ADD9872334-7-Swim_Instructor',     "Invalid"],
        [b'ADD9872334-7- Swim_Instructor',    "Invalid"],
        #
        [b'ADD-0000000-1-12345',                "Valid"],
        [b'ADD-9999999-9-12345',                "Valid"],
        # Job Title Boundarys
        [b'ADD-1111111-1-123',                  "Valid"],
        [b'ADD-2222222-2-12345678901234567',    "Valid"],
        [b'ADD-0000001-1-12',                 "Invalid"],
        [b'ADD-0000002-1-123456789012345678',  "Invalid"],
        [b'ADD-0000001-1-',                   "Invalid"],

        # Job Grade Invalid values
        [b'ADD-0000003-A-12345',              "INVALID"],
        [b'ADD-0000000-0-12345',              "Invalid"],
        # Invalid Fomatting (hyphens)
        [b'ADD0000006112345',                 "Invalid"],
        [b'ADD 0000007-1-12345',              "Invalid"],
        [b'ADD-0000008_1-12345',              "Invalid"],
        [b'ADD-0000009-1+12345',              "Invalid"],
        [b'ADD-00 0016-1-12345',              "Invalid"],
        # Wrong amount of nums
        [b'ADD-123456-1-12345',               "Invalid"],
        [b'ADD-12345678-1-12345',             "Invalid"],
        [b'ADD-0000004-11-12345',             "Invalid"],
        [b'ADD-0000005--12345',               "Invalid"],
        [b'ADD-0000006-11-12345',             "Invalid"],
        # Invalid characters
        [b'ADD-0000010-1-12345@',             "Invalid"],
        [b'ADD-0000011-1-12345 ',             "Invalid"],
        [b'ADD-0000012-1-12345-',             "Invalid"],
        [b'ADD-0000013-1-1>345<',             "Invalid"],
        [b'ADD-A000014-1-12345_',             "Invalid"],
        # Duplicate Data
        [b'ADD-0000000-1-12345_',             "Invalid"],  

        # Valid Test Data -> 5
        [b'ADD-6359870-6-Software_Engineer',    "Valid"],
        [b'ADD-3356872-4-Guard',                "Valid"],
        [b'ADD-1425466-8-HR.Manager',           "Valid"],
        [b'ADD-2357712-1-Care_Taker',           "Valid"],
        [b'ADD-3356872-7-Swim_Instructor',       "Invalid"],
        
        [b'ADD-0000015-9-TO_DELETE',            "Valid"],
        [b'ADD-0000017-9-TO_EDIT_DEL',          "Valid"],

        [b'ADD-2000001-1-Guard',            "Valid"],
        [b'ADD-2000002-1-Guard',            "Valid"],
        [b'ADD-2000003-1-Guard',            "Valid"],
        [b'ADD-2000004-1-Guard',            "Valid"],
        [b'ADD-2000005-1-Guard',            "Valid"],
        [b'ADD-2000006-1-Guard',            "Valid"],
        [b'ADD-2000007-1-Guard',            "Valid"],
        [b'ADD-2000008-1-Guard',            "Valid"],
        [b'ADD-2000009-1-Guard',            "Valid"],
        [b'ADD-2000010-1-Guard',            "Invalid"],
        
        ]

pst = [ [b'PST',                             "Invalid"],
        [b'PST-',                            "Invalid"],

        # Boundary for length of ID.
        [b'PST-123456-NPEN',                      "Invalid"],
        [b'PST-12345678-NPEN',                    "Invalid"],

        # Invalid Characters/ID Format.
        [b'PST- 123456-NPEN',                     "Invalid"],
        [b'PST-123456_-NPEN',                     "Invalid"],
        [b'PST--123456-NPEN',                     "Invalid"],
        [b'PST-123456--NPEN',                     "Invalid"],
        [b'PST-123@456-NPEN',                     "Invalid"],
        [b'PST-1234A56-NPEN',                     "Invalid"],
        [b'PST1234A56-NPEN',                      "Invalid"],
        # Format
        [b'PST-9999998- NPEN',                     "Invalid"],
        [b'PST-9999998-NPEN ',                     "Invalid"],
        [b'PST-9999998_NPEN',                      "Invalid"],

        # NOT PEN OR NPEN (Longth & Characters)
        [b'PST-1000001-PENN',           "Invalid"],
        [b'PST-1000001-PN',             "Invalid"],
        [b'PST-1000001-',               "Invalid"],
        [b'PST-1000001-1',              "Invalid"],

        # Employee Doesn't exist
        [b'PST-9999998-PENN',           "Invalid"],

        # Same Pension status
        [b'PST-2357712-PEN',            "Invalid"],

        # Salary is 0 -> ADD DEBUG MESSAGE PENSION STATUS NOT CHANGED
        [b'PST-0000015-NPEN',           "Invalid"],

        # Double Edit on show.
        [b'PST-1000001-NPEN',            "Valid"],
        [b'PST-1000001-PEN',             "Valid"],
        
        # 3 Valid Edits
        [b'PST-0000017-NPEN',            "Valid"],
        [b'PST-3356872-NPEN',            "Valid"],
        [b'PST-1425466-NPEN',            "Valid"],
        [b'PST-1425466-NPEN',            "Invalid-already NPEN"],
      ]

grd = [ [b'GRD',                             "Invalid"],
        [b'GRD-',                            "Invalid"],

        # Boundary for length of ID.
        [b'GRD-123456-2',                      "Invalid"],
        [b'GRD-12345678-2',                    "Invalid"],

        # Invalid Characters/ID Format.
        [b'GRD- 123456-2',                     "Invalid"],
        [b'GRD-123456_-2',                     "Invalid"],
        [b'GRD--123456-2',                     "Invalid"],
        [b'GRD-123456--2',                     "Invalid"],
        [b'GRD-123@456-2',                     "Invalid"],
        [b'GRD-1234A56-2',                     "Invalid"],
        [b'GRD1234A56-2',                      "Invalid"],
        # Format
        [b'GRD-9999998- 2',                     "Invalid"],
        [b'GRD-9999998-2 ',                     "Invalid"],
        [b'GRD-9999998_2',                      "Invalid"],

        # Boundary job Grade (0, 10,1,9)
        [b'GRD-1111111-0',                      "Invalid"],
        [b'GRD-1111111-1',                      "Invalid"], # Would never be valid if minimum is 1 and cannot be the same.
        [b'GRD-1111111-10',                     "Invalid"],

        # Employee Doesn't exist
        [b'GRD-9999998-2',                      "Invalid"],

        # Same then lower
        [b'GRD-1111111-1',                      "Invalid"],
        [b'GRD-9999999-5',                      "Invalid"],

        # Higher Valid data twice
        [b'GRD-6359870-9',                      "valid"],
        [b'GRD-3356872-9',                      "valid"],
        [b'GRD-2357712-5',                      "valid"],
        
      ]

sal = [ [b'SAL',                             "Invalid"],
        [b'SAL-',                            "Invalid"],

        # Boundary for length of ID.
        [b'SAL-123456-2',                      "Invalid"],
        [b'SAL-12345678-2',                    "Invalid"],

        # Invalid Characters/ID Format.
        [b'SAL- 123456-2',                     "Invalid"],
        [b'SAL-123456_-2',                     "Invalid"],
        [b'SAL--123456-2',                     "Invalid"],
        [b'SAL-123456--2',                     "Invalid"],
        [b'SAL-123@456-2',                     "Invalid"],
        [b'SAL-1234A56-2',                     "Invalid"],
        [b'SAL1234A56-2',                      "Invalid"],

        # Length of salary
        [b'SAL-1234567-',                      "Invalid"],
        [b'SAL-1234568-100000.00',             "Invalid"],
        [b'SAL-2222222-100000.000',            "Invalid"],

        # Node doesn't exist.
        [b'SAL-9999998-12000',       "Invalid"],


        # Number operations  all wrong
        [b'SAL-3356872-99999.99',       "Valid"], #/?
        [b'SAL-1000001-31300',              "Valid"], #?
        [b'SAL-3356872-9.999.99',       "Invalid"],
        [b'SAL-1111111-.12',       "Valid"],
        [b'SAL-2222222-.156712',       "Valid"],

        [b'SAL-2357712-10.1561',       "Valid"],
        [b'SAL-6359870-20000.156',       "Valid"],
        [b'SAL-1425466-10000.',       "Valid"],
        [b'SAL-3356872-45000',       "Valid"],
        [b'SAL-0000017-71250',       "Valid"],
        [b'SAL-1000001-0',       "Valid"],
        [b'SAL-1000001-0.00',       "Valid"],
        [b'SAL-1000001-9.996',       "Valid"],
        [b'SAL-1000001-0.996',       "Valid"],
        [b'SAL-1000001-5000.1',       "Valid"],
        [b'SAL-1000001-3100.999',       "valid"],

        [b'SAL-9999999-99999.999',       "Invalid"],
        [b'SAL-9999999-99999.99',       "valid"],
      ]

cjt = [ [b'CJT',                             "Invalid"],
        [b'CJT-',                            "Invalid"],

        # Boundary for length of ID.
        [b'CJT-123456-12345',                      "Invalid"],
        [b'CJT-12345678-12345',                    "Invalid"],

        # Invalid Characters/ID Format.
        [b'CJT- 123456-12345',                     "Invalid"],
        [b'CJT-123456_-12345',                     "Invalid"],
        [b'CJT--123456-12345',                     "Invalid"],
        [b'CJT-123456--12345',                     "Invalid"],
        [b'CJT-123@456-12345',                     "Invalid"],
        [b'CJT-1234A56-12345',                     "Invalid"],
        [b'CJT1234A56-12345',                      "Invalid"],

        # Node which does not exist.
        [b'CJT-9999998-12345',                     "Invalid"],

        # Format
        [b'CJT-9999998- 12345',                     "Invalid"],
        [b'CJT-9999998-12345 ',                     "Invalid"],
        [b'CJT-9999998_123456',                     "Invalid"],

        # Job Title boundaries & characters (See ADD)
        # Job Title Boundarys
        [b'CJT-9999998-123',                  "InValid - should catch on emp not length"],
        [b'CJT-9999998-12345678901234567',    "InValid - should catch on emp not length"],
        [b'CJT-0000001-12',                 "Invalid"],
        [b'CJT-0000002-123456789012345678',   "Invalid"],
        [b'CJT-0000001-',                   "Invalid"],

        [b'CJT-0000010-12345@',             "Invalid"],
        [b'CJT-0000011-12345 ',             "Invalid"],
        [b'CJT-0000012-12345-',             "Invalid"],
        [b'CJT-0000013-1>345<',             "Invalid"],

        # Employee Doesn't exist
        [b'CJT-9999998-12345',                     "Invalid"],

        # Valid job change twice (to CHANGE1 or Change 2 etc)
        [b'CJT-3356872-Software_Engineer',       "Valid"],
        [b'CJT-0000017-CHANGE1',                 "Valid"],
        [b'CJT-1000001-CHANGE2',                 "Valid"],
        [b'CJT-1000001-CHANGE3',                 "Valid"],
        [b'CJT-2000001-Longer1',                 "Valid"],
        [b'CJT-2000001-Longer11',                "Valid"],
      ]

# check freeram after each one (debug print to show that it has actually been deleted)
delete=[[b'DEL',                             "Invalid"],
        [b'DEL-',                            "Invalid"],

        # Boundary for length of ID.
        [b'DEL-123456',                      "Invalid"],
        [b'DEL-12345678',                    "Invalid"],

        # Invalid Characters/ID Format.
        [b'DEL- 123456',                     "Invalid"],
        [b'DEL-123456_',                     "Invalid"],
        [b'DEL--123456',                     "Invalid"],
        [b'DEL-123456-',                     "Invalid"],
        [b'DEL-123@456',                     "Invalid"],
        [b'DEL-1234A56',                     "Invalid"],
        [b'DEL1234A56',                      "Invalid"],

        # Node which does not exist.
        [b'DEL-9999998',                     "Invalid"],

        # Node which has been edited.
        [b'DEL-0000017',                      "Valid"],

        # Node made to be deleted
        [b'DEL-0000015',                      "Valid"],

        #Active ROM
        [b'ROM-code', "Valid"]
       ]

eeprom = [
          #Write some valid nodes 5
        [b'ADD-1000001-1-Guard', "Valid"],
        [b'ADD-9999999-1-Guard', "Valid"],
        [b'ADD-0000000-1-Guard', "Valid"],
        [b'ADD-0000001-1-Guard', "Valid"],
        [b'ADD-0000002-1-Guard', "Valid"],

        #Active ROM
        [b'ROM-code', "Valid"],



]

# Simply write these messages out one by one
# Customise above and below as you see fit.

msgs = general + add + sal + pst +  grd + cjt + delete
#msgs = eeprom
#msgs = add + sal
#msgs = add +cjt
#msgs = add + delete
#msgs = [msg for sublist in msgs for msg in sublist]  # Flatten the list
print(msgs)
#msgs = []
def test():
    for msg in msgs:
        print(RESET + "Write:", str(msg[0]), " Expected: ", msg[1])
        ser.write(msg[0] + b'\n') # Add Newline control character after each message
        still_reading = True
        while(still_reading):
            # Check for message back.  This will timeout after args.timeout seconds
            line = ""
            line = str(ser.readline())
            if (line[2:8].upper() == 'DEBUG:'): # Remember to send a DONE! message after sending all your DEBUG: messages
                print("Read: ", line)            
            elif (line[2:8].upper() == 'ERROR:'): # You do not need to send a DONE! message after sending an ERROR: message
                if msg[1] == "Invalid" or msg[1] == "INVALID" or msg[1] == "InValid" or msg[1]== "Invalid-already NPEN" or msg[1] == "Invalid - should catch on emp not length" or msg[1] == "InValid - should catch on emp not length": 
                    print(GREEN + "Read: ", line)
                else:
                    print(RED + "Read: ", line)
                still_reading = False
            elif (line[2:7].upper() == 'DONE!'):  # A DONE! message will make the python testing program send the next msg in msgs  
                if msg[1] == "Valid" or msg[1] == "valid":
                    print(GREEN + "DONE!")
                else:
                    print(RED + "DONE!")        
                still_reading = False
            else:
                print(RED + "Read: ", line)

    print(RESET + "Finished")

test()

delALL = [[b'DEL-1000001', "Valid"],
          [b'DEL-0000000', "Valid"],
          [b'DEL-9999999', "Valid"],
          [b'DEL-1111111', "Valid"],
          [b'DEL-2222222', "Valid"],
          [b'DEL-6359870', "Valid"],
          [b'DEL-3356872', "Valid"],
          [b'DEL-1425466', "Valid"],
          [b'DEL-2357712', "Valid"],
          [b'DEL-2000009', "Valid"],
          [b'DEL-2000001', "Valid"],
          [b'DEL-2000002', "Valid"],
          [b'DEL-2000003', "Valid"],
          [b'DEL-2000004', "Valid"],
          [b'DEL-2000005', "Valid"],
          [b'DEL-2000006', "Valid"],
          [b'DEL-2000007', "Valid"],
          [b'DEL-2000008', "Valid"],
          
          ]
# as soon as anything input into python script run test again
user_input = input("Press Enter to run all delete tests")
msgs = delALL
test()

user_input = input("Press Enter to run add some nodes again tests")
msgs = [[b'ADD-1111111-1-Guard', "Valid"],
        [b'ADD-9999999-1-Guard', "Valid"],
        [b'ADD-0000000-1-Guard', "Valid"]]
test()

