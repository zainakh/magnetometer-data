import serial
import os
import threading

no_input = True

def collect():
    global no_input
    
    # open serial port that Arduino is using   
    ser = serial.Serial("COM7:", 115200)

    #raw_path = 'Z:\Jimmy2\Zain\Test'
    raw_path = "C:\\Users\\TDT\\Desktop"
    filename_part = 'sporadic14'
    filename = filename_part + '.txt'
    filename_csv = filename_part + '.csv'
    
    filepath = os.path.join(raw_path, filename)
    filepath_csv = os.path.join(raw_path, filename_csv)
    
    if not os.path.exists(raw_path):
        os.makedirs(raw_path)
    
    if os.path.isfile(filepath) or os.path.isfile(filepath_csv):
        print("File Exists")
        return
    
    print("File created/opened")    
    f = open(filepath, "w")
        
    # Possible time or input control
    X_found = False
    while(no_input):
        ser.write('a'.encode())
        ser.flush()
        if(X_found == False):
            line_bytes = ser.readline()
            line_space = line_bytes.decode("utf-8", "backslashreplace")
            
            if("X0" not in line_space):
                line_space = ""
            else:
                X_found = True
                index_X = line_space.find('X')
                line_space = line_space[index_X:]
                
        else:
            line_bytes = ser.readline()
            #line_space = str(line_bytes)
            line_space = str(line_bytes, "utf-8")
        
        line = line_space.rstrip('\n')                    
        f.write(line)

    ser.flush()
    ser.write('b'.encode())
    ser.flush()
    
    f.close()
    print("File closed")
    
    # Convert to CSV file
    raw_path += '\\'
    full_path = raw_path + filename

    base = os.path.splitext(full_path)[0]
    os.rename(full_path, base + ".csv") 
    
    no_input = True


def signal_user_input():
    global no_input
    i = input("Press Enter to stop\n")
    no_input = False


def start():    
    threading.Thread(target = signal_user_input).start()
    collect()

    
