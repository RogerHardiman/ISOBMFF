/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Jean-David Gadina - www.xs-labs.com / www.imazing.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @file        ILOC.cpp
 * @copyright   (c) 2017, Jean-David Gadina - www.xs-labs.com / www.imazing.com
 */

#include <ISOBMFF/ILOC.hpp>

template<>
class XS::PIMPL::Object< ISOBMFF::ILOC >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint8_t                                               _offsetSize;
        uint8_t                                               _lengthSize;
        uint8_t                                               _baseOffsetSize;
        uint8_t                                               _indexSize;
        std::vector< std::shared_ptr< ISOBMFF::ILOC::Item > > _items;
};

#define XS_PIMPL_CLASS ISOBMFF::ILOC
#include <XS/PIMPL/Object-IMPL.hpp>

namespace ISOBMFF
{
    ILOC::ILOC( void ): FullBox( "iloc" )
    {}
    
    void ILOC::ReadData( Parser & parser, BinaryStream & stream )
    {
        uint8_t  u8;
        uint32_t count;
        uint32_t i;
        
        FullBox::ReadData( parser, stream );
        
        u8 = stream.ReadUInt8();
        
        this->SetOffsetSize( u8 >> 4 );
        this->SetLengthSize( u8 & 0xF );
        
        u8 = stream.ReadUInt8();
        
        this->SetBaseOffsetSize( u8 >> 4 );
        this->SetIndexSize( u8 & 0xF );
        
        if( this->GetVersion() < 2 )
        {
            count = stream.ReadBigEndianUInt16();
        }
        else
        {
            count = stream.ReadBigEndianUInt32();
        }
        
        this->impl->_items.clear();
        
        for( i = 0; i < count; i++ )
        {
            this->AddItem( std::make_shared< Item >( stream, *( this ) ) );
        }
    }
    
    void ILOC::WriteDescription( std::ostream & os, std::size_t indentLevel ) const
    {
        FullBox::WriteDescription( os, indentLevel );
        DisplayableObjectContainer::WriteDescription( os, indentLevel );
    }
    
    std::vector< std::shared_ptr< DisplayableObject > > ILOC::GetDisplayableObjects( void ) const
    {
        auto v( this->GetItems() );
        
        return std::vector< std::shared_ptr< DisplayableObject > >( v.begin(), v.end() );
    }
    
    std::vector< std::pair< std::string, std::string > > ILOC::GetDisplayableProperties( void ) const
    {
        auto props( FullBox::GetDisplayableProperties() );
        
         props.push_back( { "Offset size",      std::to_string( this->GetOffsetSize() ) } );
         props.push_back( { "Length size",      std::to_string( this->GetLengthSize() ) } );
         props.push_back( { "Base offset size", std::to_string( this->GetBaseOffsetSize() ) } );
           
        if( this->GetVersion() == 1 || this->GetVersion() == 2 )
        {
            props.push_back( { "Index size", std::to_string( this->GetIndexSize() ) } );
        }
        
        props.push_back( { "Items", std::to_string( this->GetItems().size() ) } );
        
        return props;
    }
    
    uint8_t ILOC::GetOffsetSize( void ) const
    {
        return this->impl->_offsetSize;
    }
    
    uint8_t ILOC::GetLengthSize( void ) const
    {
        return this->impl->_lengthSize;
    }
    
    uint8_t ILOC::GetBaseOffsetSize( void ) const
    {
        return this->impl->_baseOffsetSize;
    }
    
    uint8_t ILOC::GetIndexSize( void ) const
    {
        return this->impl->_indexSize;
    }
    
    void ILOC::SetOffsetSize( uint8_t value )
    {
        this->impl->_offsetSize = value;
    }
    
    void ILOC::SetLengthSize( uint8_t value )
    {
        this->impl->_lengthSize = value;
    }
    
    void ILOC::SetBaseOffsetSize( uint8_t value )
    {
        this->impl->_baseOffsetSize = value;
    }
    
    void ILOC::SetIndexSize( uint8_t value )
    {
        this->impl->_indexSize = value;
    }
    
    std::vector< std::shared_ptr< ILOC::Item > > ILOC::GetItems( void ) const
    {
        return this->impl->_items;
    }
	
	void ILOC::AddItem( std::shared_ptr< Item > item )
	{
        this->impl->_items.push_back( item );
    }
}

XS::PIMPL::Object< ISOBMFF::ILOC >::IMPL::IMPL( void ):
    _offsetSize( 0 ),
    _lengthSize( 0 ),
    _baseOffsetSize( 0 ),
    _indexSize( 0 )
{}

XS::PIMPL::Object< ISOBMFF::ILOC >::IMPL::IMPL( const IMPL & o ):
    _offsetSize( o._offsetSize ),
    _lengthSize( o._lengthSize ),
    _baseOffsetSize( o._baseOffsetSize ),
    _indexSize( o._indexSize ),
    _items( o._items )
{}

XS::PIMPL::Object< ISOBMFF::ILOC >::IMPL::~IMPL( void )
{}

