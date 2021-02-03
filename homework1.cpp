#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>

using namespace std;

string csvFile;
string prefixName;

bool OPEN_DATABASE = false;
ifstream Din;
ofstream DoutConfig;
ofstream DoutData;

// TesT only
ifstream DinTest;
ifstream DinData;
string OTHER;
ofstream DinDoutData;
ofstream DoutTest;


string csvConfigFile;
string csvDataFile;
string csvFinal;

string ID;
string REGION;
string STATE;
string CODE;
string NAME;
string TYPE;
string VISITORS;

int numberOfFiles;

const int TOTAL_MAX = 157;

const int ID_MAX = 8;
const int REGION_MAX = 3;
const int STATE_MAX = 3;
const int CODE_MAX = 5;
const int NAMES_MAX = 85; // + 3 - 1 // used to be 86, changing to 85 bc im adding '!' before each Name in .data file
const int TYPE_MAX = 40; //  + 3
const int VISITORS_MAX = 12; // + 3

string BLANK_RECORD = "                                                                                                                                                             ";

int ID_RECORD;
int MIDDLE_INT;

void padRecords(string &ID, string &REGION, string &STATE, string &CODE, string &NAME, string &TYPE, string &VISITORS){
        int appendNum;

        appendNum = ID_MAX - ID.length();
        for(int a=0; a<appendNum; a++)
            ID = ID + " ";

        appendNum = REGION_MAX - REGION.length();
        for(int b=0; b<appendNum; b++)
            REGION = REGION + " ";

        appendNum = STATE_MAX - STATE.length();
        for(int c=0; c<appendNum; c++)
            STATE = STATE + " ";

        appendNum = CODE_MAX - CODE.length();
        for(int d=0; d<appendNum; d++)
            CODE = CODE + " ";

        appendNum = NAMES_MAX - NAME.length();
        for(int e=0; e<appendNum; e++)
            NAME = NAME + " ";


        appendNum = TYPE_MAX - TYPE.length();
        for(int f=0; f<appendNum; f++)
            TYPE = TYPE + " ";

        appendNum = VISITORS_MAX - VISITORS.length();
        for(int g=0; g<appendNum; g++)
            VISITORS = VISITORS + " ";
}


void createDatabase(){
    // TODO
    if(!OPEN_DATABASE){
        cout << "\n [] Enter the name of a .csv file (Exclude extension, e.g., 'Parks'): ";
        cin >> csvFile;
            // HERE MIGHT NEED TO ADD SOME ERROR CHECKING TO MAKE SURE THE .CSV FILE EXISTS IN THE DIRECTORY
        cout << "\n + Two new files have been made: " << "\n    []" << csvFile << ".config" << "\n    []" << csvFile << ".data\n";
        // Read and convert the .csv file into two files: Parks.config and Parks.data
        // This can be specific to a csv file that has exactly the 7 fields in our datafile
        // YOu may assume that the records in the .csv file are sorted in order of the primary key
            // Parks.config: contains the number of records in the file, describes the names, sizes of the fields in order, anything els you want. The first field is assumed to be the key
            // Parks.data: contains the data records, one per line, with fixed size fields. You may use any separator you want (or no separator)
        csvConfigFile = csvFile + ".config";
        csvDataFile = csvFile + ".data";
        csvFinal = csvFile + ".csv";

        DoutConfig.open(csvConfigFile);
        DoutData.open(csvDataFile);
        Din.open(csvFinal);

        // CONFIG FILE
        string firstLine;
        string headers;

        getline(Din, firstLine);
        stringstream s(firstLine);

        vector<string> firstRow;

        while(getline(s, headers, ',')){
            firstRow.push_back(headers);
        }

        ID = firstRow[0];
        REGION = firstRow[1];
        STATE = firstRow[2];
        CODE = firstRow[3];
        NAME = firstRow[4];
        TYPE = firstRow[5];
        VISITORS = firstRow[6];

        DoutConfig << ID << "||" << REGION << "||" << STATE << "||" << CODE << "||"<< VISITORS << "||" << TYPE << "||" << NAME << endl;
        firstRow.clear(); // clear the vector to save space

        // THIS IS WHERE WE ADD INFO TO OUR .DATA FILE
        string currentLine = "";
        vector<string> FieldValues;
        while(!Din.eof()){
            string fields;

            getline(Din, currentLine);
/////////////////// here we check if the currentLine string has any quotation marks. If so we will treat it differently./////////////////////////
            vector<string> fieldsVector;
            string quotationMark = "\"";

            if(currentLine.find(quotationMark) != std::string::npos){
                int i = 0;
                string currentField;

                // get first 3 fields
                while(currentLine[i] != '\"'){
                    while(currentLine[i] != ','){
                        currentField = currentField + currentLine[i];
                        i = i + 1;
                    }
                    fieldsVector.push_back(currentField);
                    currentField = "";
                    i = i + 1;
                }

                // GET NAME
                i = i + 1;
                currentField = currentField + '\"';
                while(currentLine[i] != '\"'){
                    currentField = currentField + currentLine[i];
                    i = i + 1;
                }
                currentField = currentField + '\"';
                fieldsVector.push_back(currentField);
                currentField = "";


                i = i + 2;
                // now get the other two fields that are left
                while(currentLine[i] != '\0'){
                    while(currentLine[i] != ','){
                        currentField = currentField + currentLine[i];
                        i = i + 1;
                        if(currentLine[i] == '\0')
                            break;
                    }
                    fieldsVector.push_back(currentField);
                    currentField = "";
                    i = i + 1;
                }
                ID = fieldsVector[0];
                REGION = fieldsVector[1];
                STATE = fieldsVector[2];
                CODE = fieldsVector[3];
                NAME = fieldsVector[4];
                TYPE = fieldsVector[5];
                VISITORS = fieldsVector[6];
                fieldsVector.clear();
            }
            else {
//////////////////////////////////////////////////////////////////////////////////////////////////////////

                stringstream s(currentLine);

                while(getline(s, fields, ',')){
                    FieldValues.push_back(fields);
                }

                ID = FieldValues[0];
                REGION = FieldValues[1];
                STATE = FieldValues[2];
                CODE = FieldValues[3];
                NAME = FieldValues[4];
                TYPE = FieldValues[5];
                VISITORS = FieldValues[6];

                FieldValues.clear();
            }
        // Here we will add ' 's to each field
            padRecords(ID, REGION, STATE, CODE, NAME, TYPE, VISITORS);


        // DoutData into the .data file
            DoutData << ID << REGION << STATE << CODE << VISITORS << TYPE << "!" << NAME << endl << BLANK_RECORD << endl;

            currentLine = "";
            numberOfFiles = numberOfFiles + 1;
        }

        // .data file is being wierd and display two extra ghostly data records, so just subtract two from numberOfFiles
        numberOfFiles = numberOfFiles - 2;

        DoutConfig << numberOfFiles;

        DoutConfig.close();

        OPEN_DATABASE = true;
        prefixName = csvFile; // this is as if you had done openDatabase on the freshly made files.
    }
    else
        cout << "\n - [Please close your current database first before creating a new one]\n";
}



void openDatabase(){
    // TODO


    if (!OPEN_DATABASE){
        cout << "\n [] What's the prefix for your file? (Exclude extension, e.g., 'Parks')\n";
        cin >> prefixName;

        csvConfigFile = prefixName + ".config";
        csvDataFile = prefixName + ".data";
        csvFinal = prefixName + ".csv";

        Din.open(csvFinal);
        if(!Din)
            cout << " - ERROR. Could not open " << csvFinal << ".\n\n";
        else {
            DinTest.open(csvConfigFile);
//          DoutConfig.open(csvConfigFile);
            DoutData.open(csvDataFile, std::ios_base::app);
            Din.open(csvFinal);

            string test;
            string test2;
            DinTest >> test;
            DinTest >> test2;

            numberOfFiles = stoi(test2);
            cout << " + NUMBER OF FILES: " << numberOfFiles << endl;

            DinTest.close();

            OPEN_DATABASE = true;
            return;
        }
    }
    else{
        cout << "\n - [Please close your current database first before opening a new one]\n";
    }

}


void closeDatabase(){
    // TODO
    if(!OPEN_DATABASE)
        cout << "\n - [No database currently open]\n";
    else{
        cout << "\n + [Successfully closed database]\n";
        OPEN_DATABASE = false;

        Din.close();
        DoutConfig.close();
        DoutData.close();
    }
}



bool getRecord(ifstream &DinData, int ID_RECORD, const int middle, string &middleID){
    bool SUCCESS = false;
    int i=0;
    string TYPE_TEMP;
    string NAME_TEMP;
    OTHER = "";

    if((0 <= middle) && (middle < numberOfFiles)){
        DinData.seekg(2*middle * (TOTAL_MAX+1), ios::beg);

        DinData >> ID;
        if( !isdigit(stoi(ID)))
//        cout << "----------------------------------BLANK------------------------------------------------";

        DinData >> REGION >> STATE >> CODE >> VISITORS;
        getline(DinData, OTHER);

        while(OTHER[i] == ' ')
            i = i + 1;

        while(OTHER[i] != '!'){
            TYPE_TEMP = TYPE_TEMP + OTHER[i];
            i = i + 1;
        }

        TYPE = TYPE_TEMP;

        i = i + 1;
        while(OTHER[i] != '\0'){
            NAME_TEMP = NAME_TEMP + OTHER[i];
            i = i + 1;
        }

        NAME = NAME_TEMP;
//      cout << "      MIDDLE: " << middle << endl;
//      cout << "      ID = " << ID << "\n      REGION = " << REGION << "\n      STATE = " << STATE << "\n      CODE = " << CODE << "\n      VISITORS = " << VISITORS << "\n      TYPE" << TYPE << "\n     $
        middleID = ID;
        SUCCESS = true;
    }
    else
        cout << " - [Record " << ID_RECORD << " out of range]\n";

    return SUCCESS;
}

int binarySearch(int ID_RECORD){

    DinData.open(csvDataFile);
    // binary search part
    int low = 0;
    int high = numberOfFiles - 1;
    int middle = 0;
    string middleID;
    bool Failure = false;

    bool Found = false;
    while(!Found && (high >= low) && !Failure)
    {
        middle = (low + high) / 2;
        if(getRecord(DinData, ID_RECORD, middle, middleID))
        {
            if(stoi(middleID) == ID_RECORD){
                Found = true;
            }
            else if (stoi(middleID) < ID_RECORD){
                low = middle + 1;
            }
            else{
                high = middle - 1;
            }
        }
        else
        {
            cout << " - [Could not get record " << middle << endl;
            Failure = true;
        }
    }

    DinData.close();

    if (Found){
//      MIDDLE_INT = ID_RECORD;
        return middle;
    }
    else{
        return -1;
    }
}

void displayRecord(){
    // You must find the record via the primary key using seeks and binary search, THEN display it
    // Each field should display the name (from the .config file) and the value (from the .data file record)
    cout << "Please enter ID for the record: ";
    cin >> ID_RECORD;

    int recordNum = binarySearch(ID_RECORD);

    if (recordNum != -1){
        cout << "\n  ID: " << ID_RECORD << " found at record " << recordNum << ": \n";
        cout << "\n  REGION: " << REGION << "\n  STATE: " << STATE << "\n  CODE: " << CODE <<  "\n  VISITORS: " << VISITORS  << "\n  TYPE: " << TYPE << "\n  NAME: " << NAME << endl << endl;
    }
    else{
        cout << " - [Could not find record]\n";
    }
//    DinData.close();
}

void updateRecord(){
    // TODO
    // As a first step, you have to find the record using the same "find" routine as 4
    // Then you should display the contents and allow updates in a specified field
    // DON'T allow the updating of the primary key
    bool everythingGood = true;

    int fieldChangeNum;
    string newREGION = "TEST";
    string newSTATE = "TEST";
    string newCODE = "TEST";
    string newVISITORS = "TEST";
    string newTYPE = "TEST";
    string newNAME = "TEST";

    int tempInt;

    cout << "Please enter ID for record you would like to update: ";
    cin >> ID_RECORD;

    int recordNum = binarySearch(ID_RECORD);
    MIDDLE_INT = recordNum;

    if(recordNum != -1){
        cout << "\n  [1]  ID: " << ID << "\n  [2]  REGION: " << REGION << "\n  [3]  STATE: " << STATE << "\n  [4]  CODE: " << CODE << "\n  [5]  VISITORS: " << VISITORS << "\n  [6]  TYPE: " << TYPE << "\n  [7]  NAME: " << NAME << endl << endl;
        cout << "Which field would you like to change? (Choose number from 2 to 7): ";
        cin >> fieldChangeNum;
        while((fieldChangeNum < 2) || (fieldChangeNum > 7)){
            cout << "Please choose a number from 2 to 7: ";
            cin >> fieldChangeNum;
        }

        switch(fieldChangeNum){
            case 2:
                cout << "Please enter new REGION name: ";
                cin.ignore();
                getline(cin, newREGION);

                // if newREGION is larger than two letters, truncate down to two letters
                tempInt = REGION_MAX - 1;
                if(newREGION.length() > tempInt){
                    string temp = newREGION.substr(0, tempInt);
                    newREGION = temp;
                }
                REGION = newREGION;

                // otherwise, newREGION is smaller than two letters so add whitespace later using padRecords
                break;
            case 3:
                cout << "Please enter new STATE name: ";
                cin.ignore();
                getline(cin, newSTATE);

                tempInt = STATE_MAX - 1;
                if(newSTATE.length() > tempInt){
                    string temp = newSTATE.substr(0, tempInt);
                    newSTATE = temp;
                }
                STATE = newSTATE;

                break;
            case 4:
                cout << "Please enter new CODE name: ";
                cin.ignore();
                getline(cin, newCODE);

                tempInt = CODE_MAX - 1;
                if(newCODE.length() > tempInt){
                    string temp = newCODE.substr(0, tempInt);
                    newCODE = temp;
                }
                CODE = newCODE;

                break;
            case 5:
                cout << "Please enter new VISITORS name: ";
                cin.ignore();
                getline(cin, newVISITORS);

                tempInt = VISITORS_MAX - 3;
                if(newVISITORS.length() > tempInt){
                    string temp = newVISITORS.substr(0, tempInt);
                    newVISITORS = temp;
                }
                VISITORS = newVISITORS;

                break;
            case 6:
                cout << "Please enter new TYPE name: ";
                cin.ignore();
                getline(cin, newTYPE);

                tempInt = TYPE_MAX - 3;
                if(newTYPE.length() > tempInt){
                    string temp = newTYPE.substr(0, tempInt);
                    newTYPE = temp;
                }
                TYPE = newTYPE;

                break;

            case 7:
                cout << "Please enter new NAME name: ";
                cin.ignore();
                getline(cin, newNAME);

                tempInt = NAMES_MAX - 2;
                if(newNAME.length() > tempInt){
                    string temp = newNAME.substr(0, tempInt);
                    newNAME = temp;
                }
//              NAME = "!" + newNAME;
                NAME = newNAME;

                break;
            default:
                cout << " - [Something went wrong and we could not change that field value. Try again]\n";
                everythingGood = false;
                break;
        }

    if(everythingGood){
        padRecords(ID, REGION, STATE, CODE, NAME, TYPE, VISITORS);
        // so now the record is ready to go into .data file
        // I guess we use seekg() with MIDDLE_INT as our position (of course multipying by our TOTAL_MAX record size value)
        // then once we have our starting point we just write our new data there somehow....
        DinDoutData.open(csvDataFile, ios::out | ios::in);
//        cout << "       MIDDLE: " << MIDDLE_INT << endl;
        DinDoutData.seekp(2*MIDDLE_INT * (1+TOTAL_MAX), ios::beg);
        DinDoutData << ID << REGION << STATE << CODE << VISITORS << TYPE << "!" << NAME << endl;

        cout << " + [Record successfully updated]\n";

        DinDoutData.close();
    }
    else{
        // do nothing
    }

    }
    else
        cout << " - [Could not find record]\n";

}

void createReport(){
    // TODO
    // This should create a "human" readable file in ASCII form, which should display the first TEN records nicely formatted, in sorted order by primary key
    string OTHER = "";
    string TYPE_TEMP;
    string NAME_TEMP;
    int i = 0;

    DinData.open(csvDataFile);
    for(int j=0; j<10;j++){

        TYPE_TEMP = "";
        NAME_TEMP = "";

        i = 0;
        DinData.seekg(2*j * (TOTAL_MAX+1), ios::beg);
        DinData >> ID;
        if( !isdigit(stoi(ID)))
//        cout << "----------------------------------BLANK------------------------------------------------";

        DinData >> REGION >> STATE >> CODE >> VISITORS;
        getline(DinData, OTHER);

        while(OTHER[i] == ' ')
            i = i + 1;

        while(OTHER[i] != '!'){
            TYPE_TEMP = TYPE_TEMP + OTHER[i];
            i = i + 1;
        }

        TYPE = TYPE_TEMP;

        i = i + 1;
        while(OTHER[i] != '\0'){
            NAME_TEMP = NAME_TEMP + OTHER[i];
            i = i + 1;
        }

        NAME = NAME_TEMP;

        cout << "RECORD " << j+1 << ": " << "ID: " << ID << "\n          REGION: " << REGION << "\n          STATE: " << STATE << "\n          CODE: " << CODE << "\n          VISITORS: " << VISITORS << "\n          TYPE: " << TYPE << "\n          NAME: " << NAME << endl << endl;
    }

    DinData.close();
}

void truncateValues(string &newID, string &newREGION, string &newSTATE, string &newCODE, string &newNAME, string &newTYPE, string &newVISITORS){
        int tempInt = 0;
        bool truncatedValues = false;

        tempInt = ID_MAX - 1;
        if(newID.length() > tempInt){
            string temp = newID.substr(0, tempInt);
            newID = temp;
            truncatedValues = true;
        }

        tempInt = REGION_MAX - 1;
        if(newREGION.length() > tempInt){
            string temp = newREGION.substr(0, tempInt);
            newREGION = temp;
            truncatedValues = true;
        }

        tempInt = STATE_MAX - 1;
        if(newSTATE.length() > tempInt){
            string temp = newSTATE.substr(0, tempInt);
            newSTATE = temp;
            truncatedValues = true;
        }

        tempInt = CODE_MAX - 1;
        if(newCODE.length() > tempInt){
            string temp = newCODE.substr(0, tempInt);
            newCODE = temp;
            truncatedValues = true;
        }

        tempInt = VISITORS_MAX - 3;
        if(newVISITORS.length() > tempInt){
            string temp = newVISITORS.substr(0, tempInt);
            newVISITORS = temp;
            truncatedValues = true;
        }

        tempInt = TYPE_MAX - 3;
        if(newTYPE.length() > tempInt){
            string temp = newTYPE.substr(0, tempInt);
            newTYPE = temp;
            truncatedValues = true;
        }

        tempInt = NAMES_MAX - 2;
        if(newNAME.length() > tempInt){
            string temp = newNAME.substr(0, tempInt);
            newNAME = temp;
            truncatedValues = true;
        }

        if(truncatedValues)
            cout << " - [Some value(s) have been truncated for being too long]\n";

}

void addRecord(){
    // TODO
    cout << "\n--adding record--\n";
    // Add the record at the database by overwriting a blank record
    // If there is no blank record at the required location (i.e. it's already been filled), the database is reorganized to reinsert blank records after each real record
    // You should NOT need to have more than one or two records in memory at a time to accomplish this

    string newID = "TEST";
    string newRegion = "TEST";
    string newState = "TEST";
    string newCode = "TEST";
    string newVisitors = "TEST";
    string newType = "TEST";
    string newName = "TEST";

    bool notInteger = true;

    //get user input for new record
    while(notInteger)
    {
        notInteger = false;
        cout << "Please enter an integer for the ID: ";
        cin >> newID;

        for(int i=0; i<newID.length(); i++){
            if( isdigit(newID[i]) == false){
                notInteger = true;
                break;
            }
        }
    }

    cout << "Enter REGION value: ";
    cin >> newRegion;

    cout << "Enter STATE value: ";
    cin >> newState;

    cout << "Enter CODE value: ";
    cin >> newCode;

    cout << "Enter VISITORS value: ";
    cin >> newVisitors;

    cout << "Enter TYPE value: ";
    cin.ignore();
    getline(cin, newType);

    cout << "Enter NAME value: ";
    cin.ignore();
    getline(cin, newName);

    truncateValues(newID, newRegion, newState, newCode, newName, newType, newVisitors);

    cout << "Checking if id in use...\n\n";
    ID_RECORD = stoi(newID);
    int recordNum = binarySearch(ID_RECORD);
    MIDDLE_INT = recordNum;
    DinData.open(csvDataFile);

    int previousIndex = 0;
    bool previousRecordIsBlank = false;
    bool addedAlready = false;
    string wholeRecord;

    bool insideAddedAlready = false;

    //if record is not currently in use
    if(recordNum == -1)
    {
        cout << newID << " does NOT exist yet";
        padRecords(newID, newRegion, newState, newCode, newName, newType, newVisitors);

        for(int i = 0; i < numberOfFiles*2; i++)
        {
            if(addedAlready)
                break;

//          cout << "INSIDE FOR LOOP i: " << i << endl;
            DinData.seekg(i*(TOTAL_MAX+1), ios::beg);
            getline(DinData, wholeRecord); // instead of taking in just ID, I need to use getine() in order for it to also recognize BLANK_RECORDS

            // here we separate our ID from our getline reading
            ID = "";
            for(int j=0; j<ID_MAX; j++){
                if(wholeRecord[j] != ' ')
                    ID = ID + wholeRecord[j];
            }

            // also check if we just grabbed a BLANK_RECORD
            if(wholeRecord == BLANK_RECORD)
            {
                // don't do anything, just grab the index and go to the next i iteration
                cout << " - BLANK RECORD \n";
                previousIndex = i;
                previousRecordIsBlank = true;
            }
            else{ // otherwise its a real record and we check stuff
                cout << endl << "ID = " << ID << endl;

                // if our newID is larger than the ID we just got, we have to keep moving up until our newID is < ID
                if(stoi(newID) > stoi(ID))
                {
                    // do nothing, maybe get the temp index?
                    previousIndex = i;
                    previousRecordIsBlank = false;
                }
                else
                {

                    addedAlready = true;
                    // now we've gone too far so we should insert our newID and the other data in the previous line
                    // currently, our index points to the record (real or blank) before this, so we can simply check if the spot is blank and insert
                    // if not blank, do some wacky stuff
//                  cout << " - - check if previous blank\n";
                    if(previousRecordIsBlank){
                        cout << " ++++++++++++++++++ ADDING FILE \n";
                        DinDoutData.open(csvDataFile, ios::out | ios::in);
                        DinDoutData.seekp(previousIndex*(1+TOTAL_MAX), ios::beg);
                        DinDoutData << newID << newRegion << newState << newCode << newVisitors << newType << "!" << newName;
                        DinDoutData.close();

                        addedAlready = true;
                    }
                    else 
                    {
                        cout << "rearranging files...\n\n";
                        //need to rearrange file to add blank spaces after each record
                        string tempFile = "Temp.data";
                        string tempLine;
                        int tempNumberOfFiles = 0;
                        DoutTest.open(tempFile, ios:: out | ios::app);
                        int j = 0;
                        while(!DinData.eof() && (j <= (2*numberOfFiles)))
                        {
                           //cout << " + + + + +  INSIDE WHILE: j = " << j << endl;
                            //used the seek from earlier to get each line
                            DinData.seekg(j*(TOTAL_MAX+1), ios::beg);
                            getline(DinData, tempLine);                            
                            //cout << tempLine << endl;
                            if(tempLine != BLANK_RECORD)
                            {
                                //insert line we got from .data file to temp and add blank
                                //after each record
                                //maybe this isn't the right way?
                                DoutTest << tempLine << endl << BLANK_RECORD << endl;
                                tempNumberOfFiles = tempNumberOfFiles + 1;
                                tempLine = "";
                            }
                            //if it is a blank line, don't deal with it
                            else
                            {
                                tempLine = "";
                            }
                            j++;
                        }
                        cout << "done transferring, now renaming\n\n";
                        //have to close the files before renaming i think, could be wrong
                        DinDoutData.close();
                        DoutTest.close();
                        //trying to remove old .data and rename temp to the new .data
                        // remove(csvDataFile.c_str());
                        int test = rename(tempFile.c_str(), csvDataFile.c_str());
                        DinDoutData.open(csvDataFile, ios::out | ios::in);
                        cout << "Finished rearranging\n\n";


                        // we find the available spot again since the rearranging messed up the index
                    }
                }
            }
        }
    }
    else
    {
        cout << ID << " already exists";
    }
    
    numberOfFiles = numberOfFiles + 1;
    DinData.close();
    DinDoutData.close();
}

void deleteRecord(){
    // TODO
    cout << "\n--deleting record--\n";
    // Delete the record at the database
    // As records are deleted, they are logically removed from the file (most likely just marked as "missing")
    // The "missing" records should be removed when the file is reorganized while reinserting blank records

    int deleteRecord;

    cout << "Enter ID for record to delete: \n";
    cin >> deleteRecord;

    int recordNum = binarySearch(deleteRecord);
    MIDDLE_INT = recordNum;

    if(recordNum != 1)
    {
        //cout << "deleting record " << deleteRecord << endl;
        DinDoutData.open(csvDataFile, ios::out | ios::in);
        DinDoutData.seekp(2*MIDDLE_INT * (1+TOTAL_MAX), ios::beg);
        DinDoutData << BLANK_RECORD << endl;
        DinDoutData.close();
        numberOfFiles = numberOfFiles - 1;
    }
    else
    {
        cout << "Record " << deleteRecord << " does not exist.";
    }
  
}

int main(){
    cout << "\n\n============================\nNAME: Alan Torres\nID:  010840328\n----------------------------\n\n";

    bool dontLeave = true;
    int CHOICE = 0;

    while(dontLeave){
        while(CHOICE < 1 || CHOICE > 9){
            cout << "\nPlease choose an option (1-9):\n";
            cout << "   [1] Create new database\n   [2] Open database\n   [3] Close database\n   [4] Display record\n   [5] Update record\n   [6] Create report\n   [7] Add record\n   [8] Delete record\n   [9] Quit\n\n";
            cin >> CHOICE;
        }
        switch(CHOICE){
            case 1:
                createDatabase();
                break;
            case 2:
                openDatabase();
                break;
            case 3:
                closeDatabase();
                break;
            case 4:
                displayRecord();
                break;
            case 5:
                updateRecord();
                break;
            case 6:
                createReport();
                break;
            case 7:
                addRecord();
                break;
            case 8:
                deleteRecord();
                break;
            case 9:
                dontLeave = false;
                break;
            default:
                dontLeave = false;
        }
        CHOICE = 0;
    }
    cout << "\n\nGOODBYE.\n----------------------------\n============================\n\n";
    return 0;
}