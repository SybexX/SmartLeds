#pragma once

#include <cstdint>
#include "esp_attr.h"
union Hsv;

union Rgb {
    struct __attribute__ ((packed)) {
        uint8_t g, r, b, a;
    };
    uint32_t value;

    Rgb( uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255 ) : g( g ), r( r ), b( b ), a( a ) {}
    Rgb( const Hsv& c );
    Rgb(const Rgb&) = default;
    Rgb& operator=( const Rgb& rgb ) { swap( rgb ); return *this; }
    Rgb& operator=( const Hsv& hsv );
    Rgb operator+( const Rgb& in ) const;
    Rgb& operator+=( const Rgb& in );
    Rgb operator-(const Rgb& in) const;
    Rgb &operator-=(const Rgb& in);
    bool operator==(const Rgb& in ) const { return in.value == value; }
    Rgb& blend( const Rgb& in );
    void swap( const Rgb& o ) {  value = o.value; }
    void linearize() {
        r = channelGamma(r);
        g = channelGamma(g);
        b = channelGamma(b);
    }

    uint8_t getGrb( int idx ) const;

    void stretchChannels( uint8_t maxR, uint8_t maxG, uint8_t maxB ) {
        r = stretch( r, maxR );
        g = stretch( g, maxG );
        b = stretch( b, maxB );
    }

    void stretchChannelsEvenly( uint8_t max ) {
        stretchChannels( max, max, max );
    }

private:
    uint8_t stretch( int value, uint8_t max ) {
        return ( value * max ) >> 8;
    }

    uint8_t channelGamma( int channel ) {
        /* The optimal gamma correction is x^2.8. However, this is expensive to
         * compute. Therefore, we use x^3 for gamma correction. Also, we add a
         * bias as the WS2812 LEDs do not turn on for values less than 4. */
        if (channel == 0)
            return channel;
        channel = channel * channel * channel * 251;
        channel >>= 24;
        return static_cast< uint8_t >( 4 + channel );
    }
};

union Hsv {
    struct __attribute__ ((packed)) {
        uint8_t h, s, v, a;
    };
    uint32_t value;

    Hsv( uint8_t h, uint8_t s = 0, uint8_t v = 0, uint8_t a = 255 ) : h( h ), s( s ), v( v ), a( a ) {}
    Hsv( const Rgb& r );
    Hsv& operator=( const Hsv& h ) { swap( h ); return *this; }
    Hsv& operator=( const Rgb& rgb );
    bool operator==( const Hsv& in ) const { return in.value == value; }
    void swap( const Hsv& o ) { value = o.value; }
};
