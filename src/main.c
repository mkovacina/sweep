#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int MAX_FILENAME_LENGTH = 255;

struct ConfigurationFiles
{
    char FsaFileName[MAX_FILENAME_LENGTH];
    char AgentFileName[MAX_FILENAME_LENGTH];
    char AgentFunctionFileName[MAX_FILENAME_LENGTH];
    char SwarmFileName[MAX_FILENAME_LENGTH];
    char SupportGridsFileName[MAX_FILENAME_LENGTH];
};

struct ConfigurationFiles* ReadConfigurationFromFile(const char* configurationFileName);
struct ConfigurationFiles* BuildConfigurationFileFromName(const char* baseConfigurationFileName);

int main( int argc, const char* argv[] )
{
    struct ConfigurationFiles* configurationFiles;
    
    switch(argc)
    {
        case 3:
        {
            int doesFileExist = access(argv[1],F_OK);
            
            if (doesFileExist)
            {
                configurationFiles = ReadConfigurationFromFile(argv[1]);
            }
            else
            {
                configurationFiles = BuildConfigurationFileFromName(argv[1]);
            }
            
        }
            break;
            
        case 7:
        {
            
        }
            break;
            
        default:
        {
            puts("Usage: sweep (<filename>|<list of files>) <random number>");
            return -1;
        }
    }
}


struct ConfigurationFiles* ReadConfigurationFromFile(const char* configurationFileName)
{
    /*
    file_names = malloc( sizeof( char* ) * 5 );
    for ( i = 0; i < 5; i++ ) file_names[i] = malloc( sizeof(char) * MAX_BUFFER ) ;
    
    TraceVerboseLine("Files read in from filename file");
    for ( i = 0; i < 5; i++ ) {
        
        fgets( file_names[i], MAX_BUFFER, FILE_NAME_FILE );
        TraceVerboseLine(file_names[i]);
    }
    
    fclose( FILE_NAME_FILE );
    
    clear_nls( file_names, 5 );
    assign_fptrs( file_names, 0, 5 );
   */
    return NULL;
}

struct ConfigurationFiles* BuildConfigurationFileFromName(const char* baseConfigurationFileName)
{
    struct ConfigurationFiles* configurationFiles = malloc(sizeof(struct ConfigurationFiles));
    
    strcat(configurationFiles->FsaFileName, ".fsa");
    strcat(configurationFiles->FsaFileName, ".agt");
    strcat(configurationFiles->FsaFileName, ".swm");
    strcat(configurationFiles->FsaFileName, ".lst");
    strcat(configurationFiles->FsaFileName, ".sgd");
    
    return configurationFiles;
}