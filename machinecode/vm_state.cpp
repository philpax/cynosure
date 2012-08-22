#include "vm_state.hpp"
#include <boost/foreach.hpp>

vm_state::vm_state( std::string filename, uint32_t memorySize, std::vector<std::string> hardDrives ) : memorySize(memorySize)
{
    log.open( filename );
    memory = new uint8_t[memorySize];
    BOOST_FOREACH( std::string hd, hardDrives )
    {
        std::fstream *file = new std::fstream( hd, std::fstream::in | std::fstream::out | std::fstream::binary );
        HDD.push_back( file );
    }
}

vm_state::~vm_state( )
{
    log << "Shutting down VM!" << std::endl;
    BOOST_FOREACH( std::fstream *hd, HDD )
    {
        hd->close();
    }
    HDD.clear();
    delete[] memory;
    log.close();
}

void vm_state::LogRegisters( )
{
    vm_state *state = this;

    for (int i = 0; i < 9; i++)
       LOG_STREAM << R_Gn(i) << ": " << PRINT_VALUE( R_G(i).r ) << " ";
    LOG_STREAM << std::endl;
}
