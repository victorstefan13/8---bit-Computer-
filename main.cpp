#include "mbed.h"

//prototype functions
void write();
void clearMemory();
void clearScreen();
void enableRead();
void disableRead();
int hexToDecimal(char usrData[]);

//declare the Bus used for the address of the EEPROM chip and DATA
BusOut address(p24,p23,p22,p21); //A0 - A3 on the EEPROM chip
BusOut data(p20, p19, p18, p17, p16, p15, p14, p13); // I/O0 - I/O7 on the EEPROM chip

//declare the digital out for the chip enable, output enable and write enable
DigitalOut chipEnable (p25); 
DigitalOut outputEnable (p26);
DigitalOut writeEnable (p27);

//set-up serial pc communications
Serial pc(USBTX, USBRX);

//declare the variables used for the Address, data and the decimal value for the data
unsigned int usrAddress, dec_val; 
char usrData[2];  

int main()
{ 
    chipEnable = 0; //set the chip enable low
    outputEnable = 1; //set output enable high
    
    writeEnable = 1; //set write enable high
    
    char choice; //varuable used for the user choice
    
    do //do the following
    {   
        //call the clearScreen function and display the menu to the user 
        clearScreen();   
        pc.printf("\n");
        pc.printf("Please select one of the following; \n");
        pc.printf("a) Write \n");
        pc.printf("b) Clear Memory \n");
        pc.printf("c) Enter Read Mode \n");
        pc.printf("x) quit \n");
        pc.printf("\n");
        
        //get user input
        pc.scanf("%c", &choice); 
        
        pc.printf("\n");
        
        //use a switch statment for the menu
        switch(choice)
        {
            case 'A':
            case 'a':
                        //call the write function
                        write();
                        break; 
            case 'B':
            case 'b':
                        //call the clearMemory function
                        clearMemory();
                        break;
            case 'C':
            case 'c':
                        //call the enableRead function
                        enableRead();
                        break;
            case 'X':
            case 'x':   
                        //call the clearScreen function 
                        clearScreen();
                        break; 
            default: 
                        //infrom the user the choice is invalid
                        pc.printf("Wrong input, please select one of the above\n");
                        break; 
        } 
    } while(choice != 'x'); // until user inputs 'x'
}        

void write() //function to write to the EEPROM chip
{       
        //call the clearScreen function and get the address from the user
        clearScreen(); 
        
        pc.printf("\n");
        pc.printf("Please select the address: \n");
        pc.printf("\n");
        
        pc.scanf("%d", &usrAddress);
        pc.printf("\n");
        
        //check to see if the address is correct
        if(usrAddress>=1 && usrAddress<=15)
        {
            address = usrAddress; //set the address buss to the address inputed by the user
            
            pc.printf("\n");
            pc.printf("Please input the data you wish to write in hexidecimal\n"); 
            pc.printf("\n");
            pc.scanf("%s", usrData);
            
            //call tge hexToDecimal function to convert the string input to an integer 
            dec_val = hexToDecimal(usrData);
            
            pc.printf("DATA : %d\n", dec_val); 
            pc.printf("\n");
            
            data = dec_val; //set the data bus to the value from usrData converted into decimal
            
            wait(2);
            
            //pulse to write to the EEPROM chip
            writeEnable= 0;
            wait(0.1); 
            writeEnable = 1;      
            
            //infor the user that the address has been writte successfully
            pc.printf("\n");
            pc.printf("Address wrote successfully\n");
            
            //reset the address and data variable ready to re use
            data = 0; 
            address = 0; 
            
            wait(1);
        }
        else
        {
            //inform the user that the input is invalid
            pc.printf("Please input a valid address (1-15)\n");
        }
}

int hexToDecimal(char usrData[]) //function to convert the string to integer
{
    int len = strlen(usrData);
    
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
    
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (usrData[i]>='0' && usrData[i]<='9') 
        { 
            dec_val += (usrData[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
        
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (usrData[i]>='A' && usrData[i]<='F') 
        { 
            dec_val += (usrData[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        }
        
        else
        {
            //inform the user that input is invalid
            pc.printf("Please input a valid value\n");
        }
    } 
    return dec_val; //return the decimal value 
} 

void clearMemory() //function to clear the EEPROM chip
{
    clearScreen(); //call the clear screen function
    
    //index through each address
    for(int j = 0; j<= 15; j++)
    {   
        //set the data for that address to 0; 
        address = j; 
        data = 0 ; 
        
        //pluse write enable to write to the EEPROM chip
        writeEnable = 0 ; 
        wait(0.5); 
        writeEnable = 1;         
    }
    //infrom the user that the memory was cleared  
    pc.printf("\n");
    pc.printf("Memory Cleared Succesfully!\n");
    
    //reset the address and data variable ready for next use
    address = 0;
    data = 0;
    wait(2);
}

void enableRead() //function to enable the user to read from the EEPROM chip
{   
    char subChoice, confirm; //declare local variables to use within the function 
    
    clearScreen(); //call the clear screen function
    
    /*confrim with the user that they wish to enter read mode and infrom them to disconnect
      the I/O on the chip so it doesnt blow the microcontroller*/
    pc.printf("Are you sure you wish to enter read mode? Please press 'y' to confirm\n");
    pc.printf("and please disconnect the I/O or press 'n' to abort!\n");
    pc.printf("\n");
    
    //get user input
    pc.scanf("%c", &confirm);
    
    if(confirm == 'y') //if user input is 'y'
    {
        //get the address from the user which they wish to read
        pc.printf("\n");
        pc.printf("Please input your address\n"); 
        pc.scanf("%d", &usrAddress);
        pc.printf("\n");
        
        address = usrAddress; //set the address bus to the user address
        
        //enable the chip for read mode
        chipEnable = 0;  //chip enable is low
        outputEnable = 0; //output enable is low
        
        writeEnable = 1; //write enable is high
        
        do 
        {  
            clearScreen(); //call the clear screen function
            
            //ask the user if the wish to input new address
            pc.printf("a) Selec a new address\n");
            pc.printf("x) Disable read mode\n"); 
            
            pc.scanf("%c", &subChoice);
            
            //use a switch statment to check though the user choice
            switch(subChoice)
            {
                case 'A':
                case 'a':
                            //get new address from user
                            pc.printf("\n");
                            pc.printf("Please input new address\n"); 
                            
                            pc.scanf("%d", &usrAddress);
                            
                            address = usrAddress; 
                            break; 
                case 'X':
                case 'x': 
                            //call the disableRead function
                            disableRead();
                            break;
                             
                default:
                            //infrom the user the selection is invalid
                            pc.printf("Please make a valid selection!\n");
                            break;
                
            }
        }while(subChoice != 'x'); //if the user inputs 'x' exit read mode
    }
    
    else
    {
        //infrom the user they will be aborting the read mode
        pc.printf("You have selected to abort read mode!\n");
        wait(2);
    } 
}

void disableRead() //function to disable read mode
{
    //infrom the user that they are exiting read mode
    pc.printf("Exiting read mode\n");
    pc.printf("\n");
    
    chipEnable = 0; //chip enable low
    outputEnable = 1; //output enable high
    
    writeEnable = 1; //write enable high
    
    wait(2); 
    
    //infrom the user they can re enable the I/O's
    pc.printf("You may now re-enable the I/O\n");
    wait(2);
}

void clearScreen() //clear the terminal screen function
{
    //this function just clears the terminal screen when called
    
    //for Tera Term/ Putty
    pc.printf("\033[2J");
    pc.printf("\033[1;1H" );
    
    //For linux/unix command line
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif
    
    //for windows command terminal
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif 
}
