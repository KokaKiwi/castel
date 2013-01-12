#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <castel/runtime/boxes/Bool.hh>
#include <castel/runtime/boxes/Class.hh>
#include <castel/runtime/boxes/Dict.hh>
#include <castel/runtime/boxes/Function.hh>
#include <castel/runtime/boxes/List.hh>
#include <castel/runtime/boxes/Null.hh>
#include <castel/runtime/boxes/Number.hh>
#include <castel/runtime/boxes/Object.hh>
#include <castel/runtime/boxes/String.hh>
#include <castel/runtime/boxes/Undefined.hh>
#include <castel/runtime/helper/create.hh>
#include <castel/runtime/helper/memoize.hh>
#include <castel/runtime/Box.hh>

#include "Compiler.hh"
#include "Program.hh"
#include "Runner.hh"
#include "Source.hh"

int Program::run( int argc, char ** argv )
{
    Source source = Source::fromFile( argv[ 1 ] );

    std::map< std::string, std::function< castel::runtime::Box * ( void ) > > globals;
    globals[ "process" ] = castel::runtime::helper::memoize( std::function< castel::runtime::Box * ( void ) >( [ ] ( ) {
        return castel::runtime::helper::create< castel::runtime::boxes::Number >( 42 );
    } ) );

    Compiler compiler;
    for ( auto & it : globals )
        compiler.globals( ).push_back( it.first );

    Runner runner = compiler.build( source );
    for ( auto & it : globals )
        runner.globals( )[ it.first ] = it.second;

    castel::runtime::Box * box = runner( );

    if ( dynamic_cast< castel::runtime::boxes::Class * >( box ) )
        std::cout << "Returned a class" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Function * >( box ) )
        std::cout << "Returned a function" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Number * >( box ) )
        std::cout << "Returned a number (" << static_cast< castel::runtime::boxes::Number * >( box )->value( ) << ")" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Bool * >( box ) )
        std::cout << "Returned a boolean (" << static_cast< castel::runtime::boxes::Bool * >( box )->value( ) << ")" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::String * >( box ) )
        std::cout << "Return a string (" << static_cast< castel::runtime::boxes::String * >( box )->value( ) << ")" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Null * >( box ) )
        std::cout << "Returned a null value" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Undefined * >( box ) )
        std::cout << "Returned an undefined value" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Dict * >( box ) )
        std::cout << "Returned a dict" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::List * >( box ) )
        std::cout << "Returned a list" << std::endl;

    if ( dynamic_cast< castel::runtime::boxes::Object * >( box ) )
        std::cout << "Returned an object" << std::endl;

    return 0;
}
