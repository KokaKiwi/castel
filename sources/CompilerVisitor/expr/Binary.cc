#include <stdexcept>

#include <llvm/Value.h>
#include <p9/lexer/Token.hh>

#include "CompilerVisitor.hh"

void CompilerVisitor::visit( p9::ast::expr::Binary & binaryExpression )
{
    if ( ! binaryExpression.left( ) )
        throw std::runtime_error( "Missing left-hand expression" );

    if ( ! binaryExpression.right( ) )
        throw std::runtime_error( "Missing right-hand expression" );

    binaryExpression.left( )->accept( *this );
    llvm::Value * left = mValue.release( );

    binaryExpression.right( )->accept( *this );
    llvm::Value * right = mValue.release( );

    switch ( binaryExpression.type( ) ) {

    case p9::lexer::TAdd:
        mValue.reset( mBuilder.CreateFAdd( left, right, "add" ) );
        break;

    case p9::lexer::TSubstract:
        mValue.reset( mBuilder.CreateFSub( left, right, "sub" ) );
        break;

    case p9::lexer::TMultiply:
        mValue.reset( mBuilder.CreateFSub( left, right, "mul" ) );
        break;

    case p9::lexer::TDivide:
        mValue.reset( mBuilder.CreateFDiv( left, right, "div" ) );
        break;

    case p9::lexer::TModulo:
        break;

    }
}
