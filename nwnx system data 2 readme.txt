v2.2
Installing:

1. Place nwnx_systemdata2.dll in the same folder nwnx.exe and nwserver.exe lives in. (typically c:/neverwinter nights/nwn/)
2. Open the module you want the plugin to use.
3. Go the temp0 folder which lives in mynwnpath/modules/temp0/. mind temp0 only exists if a mod is open in the toolset!
4. Paste nwnx_sysdata.nss into temp0.
5. Save module and reopen it, then rebuild the module!
6. Open any script you wish to use systemdata2 calls and add (#include "nwnx_sysdata") without the brackets at the top of the script.
7. Any functions with the NWNX_ tag is now useable!
8. Alternativly look at DEMO System Data 2.mod.

Functions:

//Returns TRUE if system data is responding
int NWNX_GetSystemDataAlive( );

//Returns the current second
//UNIX timestap style
//Returns a smaller variant if nShorten = TRUE
int NWNX_GetCurrentSecond( int nShorten = TRUE );

//Returns the time the server has been running in seconds
int NWNX_GetRunTime( );

//Returns the desired intiger from sFile under sSection with sKey
//If non is found it returns nDefault
//Note: the parameters may not be more then 255 characters long
int NWNX_ReadIntFromINI( string sSection, string sKey, int nDefault, string sFile );

//Returns the desired string from sFile under sSection with sKey
//If non is found it returns nDefault
//Note: the parameters may not be more then 255 characters long
string NWNX_ReadStringFromINI( string sSection, string sKey, string sDefault, string sFile );

//Returns the desired float from sFile under sSection with sKey
//If non is found it returns nDefault
//Note: the parameters may not be more then 255 characters long
float NWNX_ReadFloatFromINI( string sSection, string sKey, float fValue, string sFile );

//Returns the desired boolean from sFile under sSection with sKey
//If non is found it returns nDefault
//Note: Valid boolean types: 1/0 yes/no true/false
//Note: the parameters may not be more then 255 characters long
int NWNX_ReadBooleanFromINI( string sSection, string sKey, int nDefault, string sFile );

//Writes or changes an existing intiger in sFile
//Note: the parameters may not be more then 255 characters long
void NWNX_WriteIntToINI( string sSection, string sKey, int nValue, string sFile );

//Writes or changes an existing string in sFile
//Note: the parameters may not be more then 255 characters long
void NWNX_WriteStringToINI( string sSection, string sKey, string sValue, string sFile );

//Writes or changes an existing float in sFile
//Note: the parameters may not be more then 255 characters long
void NWNX_WriteFloatToINI( string sSection, string sKey, float fValue, string sFile );

//Writes or changes an existing boolean in sFile
//Note: the parameters may not be more then 255 characters long
void NWNX_WriteBooleanToINI( string sSection, string sKey, int nValue, string sFile );

//This function calls strftime( char * format ) via system data 2
//Definition on how to use this function can be found here:
//http://www.cplusplus.com/reference/clibrary/ctime/strftime.html
//Note: the parameters may not be more then 255 characters long
//nUnix represents which timestamp "STRFTIME" should use to generate the timestring
//is nUnix = 0 it'll take the current time
string NWNX_RealLifeClock( string sParameterString, int nUnix = 0 );

//Preforms a system command (aka CMD command)
//Definition can be found here:
//http://www.cplusplus.com/reference/clibrary/cstdlib/system.html
//Note: the parameters may not be more then 255 characters long
//WARNING! Some commands may freeze nwserver.exe until they're executed
//meaning executing commands such as "cmd" will cause the thread running
//nwserver.exe to freeze until the new window is terminated!
int NWNX_System( string sParameters );

//Returns a system environment string variable
//This function returns system environment variables
//see: http://www.cplusplus.com/reference/clibrary/cstdlib/getenv.html
//Note: the parameters may not be more then 255 characters long
string NWNX_GetEnvironmentString( string sName );

//Converts the first character in cChar to its respective ascii number
int NWNX_CharToInt( string cChar );

//Turns each of nData to their respecive ascii signs and puts them togheter
//Example: 72 69 76 76 79 = "HELLO"
string NWNX_IntToChar( int nData1, int nData2 = -1, int nData3 = -1, int nData4 = -1, int nData5 = -1 );

//Returns the first matching file with sPath
//Example C:/*.* will return the first file in C:
//Example C:/*.jpg will return the first .jpg
//Example C:/Bo.* returns the first file with the filename "Bo"
//etc
//Returns "0" on failure
//Sets the filehandle to the first file specified
//Note: the parameters may not be more then 255 characters long
string NWNX_GetFirstFile( string sPath );

//Returns the next file according to the specifics of GetFirstFile()
//Returns "0" on failure or there is no more files to show
//Sets the filehandle to the next file specified
string NWNX_GetNextFile( );

//Returns a unixstyle timestamp instead of a human readable
//GetSetFilehandle() GetFirst/NextFile() can be used to set the filehandle
//to determin which file to read off
//TIMETYPE = 1 for creation time
//TIMETYPE = 2 for last accessed time
//TIMETYPE = 3 for last write time
int NWNX_GetTimeStampFromFile( int TIMETYPE, string sFile );

//Delets s filename
//sFile should be in "c:/folder/folder/file.extention" formatting
//Note: the parameters may not be more then 255 characters long
//Returns true on success false otherwise
int NWNX_DeleteFile( string sFile );

//File copy
//sOrginalFile and sNewFile should be in "c:/folder/folder/file.extention" formatting
//Note: the parameters may not be more then 255 characters long
//Returns true on success false otherwise
int NWNX_CopyFile( string sOrginalFile, string sNewFile, int bAllowOverWriting = FALSE );

//File moving
//sOrginalFile and sNewFile should be in "c:/folder/folder/file.extention" formatting
//Note: the parameters may not be more then 255 characters long
//Returns true on success false otherwise
int NWNX_MoveFile( string sOrginalFile, string sNewFile, int bAllowOverWriting = FALSE );

//File renaming
//sFile and sNewFileName should be in "c:/folder/folder/file.extention" formatting
//Note: the parameters may not be more then 255 characters long
//Returns true on success false otherwise
int NWNX_RenameFile( string sFile, string sNewFileName );

//Strips path from a fillfile path
//Example: "c:/folder/folder/file.extention" = "file.extention"
//nGetName = TRUE and it returns: "file.extention"
//nGetName = FALSE and it returns: "c:/folder/folder/"
string NWNX_StripFilePath( string sFullFilePath, int nGetName = TRUE );

//Writes sData to sFile and breaks to the next line afterwards if nNextLine = true
int NWNX_WriteStringToTextFile( string sFile, string sData, int nNextLine = TRUE );

//Reads the entire content of sFile and returns as a string
//Suggest not using this on non textfiles or overly long ones, nwn strings has a limit.
string NWNX_ReadStringFromTextFile( string sFile );

//Sets oObjects gold worth to nValue
//Returns TRUE on success
int NWNX_SetGoldPieceValue( object oObject, int nValue );

//Get the filesize of sFile
int NWNX_GetFileSize( string sFile );

//Returns the latest matching file with sPath
//Example C:/*.* will return the first file in C:
//Example C:/*.jpg will return the first .jpg
//Example C:/Bo.* returns the first file with the filename "Bo"
//Returns "0" on failure
string NWNX_GetLatestUsedFile( string sPath );

//Returns true if sFile exists
//Should be absorlute path
//IE c:/blabla/blabla/myfile.extention
int NWNX_GetFileExists( string sFile );

//Kill sProcess
void NWNX_TerminateProcess( string sReturn );