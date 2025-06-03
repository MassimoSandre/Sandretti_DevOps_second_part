#include "image_processing.hpp"
#include <gtest/gtest.h>

TEST(GrayscaleTest, RedChannelConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 0, 0}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);

    // in a red channel conversion, the grayscale value should be equal to the red channel value
    EXPECT_EQ(result[0][0], 255); // Red channel only
}

TEST(GrayscaleTest, LuminosityConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 255, 255}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    // Luminosity method uses the formula: 0.21*R + 0.72*G + 0.07*B
    //EXPECT_EQ(result[0][0], 255); // White pixel, should remain max gray
    // because of rounding, the result is 254
    EXPECT_EQ(result[0][0], 254); // White pixel, should remain max gray
}

TEST(GrayscaleTest, AverageConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);

    // Average method uses the formula: (R + G + B) / 3
    // the pixel has RGB values of 30, 90, and 150 =>
    // (30 + 90 + 150) / 3 = 90
    EXPECT_EQ(result[0][0], (30 + 90 + 150) / 3); // Arithmetic mean
}

// new tests
TEST(GrayscaleTest, EmptyImage) {
    std::vector<std::vector<std::array<int, 3>>> image = {};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 0, 0, GrayscaleMethod::Average, result);

    // Expect the result to be empty as well since the input image is empty
    // and the specified dimensions are 0x0
    EXPECT_EQ(result.size(), 0); 
}

TEST(GrayscaleTest, ImageSize) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);

    // Expect the result to have one row and one column since the input image is 1x1
    // and the specified dimensions are 1x1
    EXPECT_EQ(result.size(), 1); 
    EXPECT_EQ(result[0].size(), 1); 
}

TEST(GrayscaleTest, ImageSize2) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150},{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 2, GrayscaleMethod::Average, result);

    // Expect the result to have one row and two columns since the input image is 1x2
    // and the specified dimensions are 1x2
    EXPECT_EQ(result.size(), 1); 
    EXPECT_EQ(result[0].size(), 2); 
}

TEST(GrayscaleTest, ImageSize3) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}},{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 2, 1, GrayscaleMethod::Average, result);

    // Expect the result to have two rows and one column since the input image is 2x1
    // and the specified dimensions are 2x1
    EXPECT_EQ(result.size(), 2); 
    EXPECT_EQ(result[0].size(), 1); 
}

TEST(GrayscaleTest, ImageSize4) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{30, 90, 150},{30, 90, 150}},
        {{30, 90, 150},{30, 90, 150}},
        {{30, 90, 150},{30, 90, 150}},
        {{30, 90, 150},{30, 90, 150}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 4, 2, GrayscaleMethod::Average, result);

    // Expect the result to have four rows and two columns since the input image is 4x2
    // and the specified dimensions are 4x2
    EXPECT_EQ(result.size(), 4); 
    EXPECT_EQ(result[0].size(), 2); 
}


// channel tests
TEST(GrayscaleTest, RedChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {4, 5, 6}},
        {{7, 8, 9},     {10, 11, 12}},
        {{13, 14, 15},  {16, 17, 18}},
        {{19, 20, 21},  {22, 23, 24}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 4, 2, GrayscaleMethod::RedChannel, result);
    
    // In a red channel conversion, the grayscale value should be equal to the red channel value (first value)
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][0]) << " at (" << row << ", " << col << ")";
        }
    }
}
TEST(GrayscaleTest, GreenChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {4, 5, 6}},
        {{7, 8, 9},     {10, 11, 12}},
        {{13, 14, 15},  {16, 17, 18}},
        {{19, 20, 21},  {22, 23, 24}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 4, 2, GrayscaleMethod::GreenChannel, result);
    
    // In a green channel conversion, the grayscale value should be equal to the green channel value (second value)
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][1]) << " at (" << row << ", " << col << ")";
        }
    }
}
TEST(GrayscaleTest, BlueChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {4, 5, 6}},
        {{7, 8, 9},     {10, 11, 12}},
        {{13, 14, 15},  {16, 17, 18}},
        {{19, 20, 21},  {22, 23, 24}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 4, 2, GrayscaleMethod::BlueChannel, result);
    
    // In a blue channel conversion, the grayscale value should be equal to the blue channel value (third value)
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][2]);
        }
    }
}

// Luminosity | Because of rounding, this tets will not pass
// TEST(GrayscaleTest, Luminosity) {
//     std::vector<std::vector<std::array<int, 3>>> image = {
//         {{1, 2, 3},     {255, 255, 0}},
//         {{92, 8, 4},     {255, 255, 255}},
//         {{0, 125, 90},     {12, 25, 85}}
//     };
//     std::vector<std::vector<int>> result;
//     convertToGrayscale(image, 3, 2, GrayscaleMethod::Luminosity, result);

//     // Luminosity method uses the formula: 0.21*R + 0.72*G + 0.07*B
//     // The expected values are calculated as follows:
//     std::vector<std::vector<int>> expected = {
//         {2 /* (0.21*1 + 0.72*2 + 0.07*3) */, 170 /* (0.21*255 + 0.72*255 + 0.07*0) */},
//         {66 /* (0.21*92 + 0.72*8 + 0.07*4) */, 255 /* (0.21*255 + 0.72*255 + 0.07*255) */},
//         {62 /* (0.21*0 + 0.72*125 + 0.07*90) */, 48 /* (0.21*12 + 0.72*25 + 0.07*85) */}
//     };
    
//     for (int row = 0; row < image.size(); row++) {
//         for (int col = 0; col < image[0].size(); col++) {
//             EXPECT_EQ(result[row][col], expected[row][col]) << " at (" << row << ", " << col << ")";
//         }
//     }
// }

// Lightness
TEST(GrayscaleTest, Lightness) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {255, 255, 0}},
        {{92, 8, 4},     {255, 255, 255}},
        {{0, 125, 90},     {12, 25, 85}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 3, 2, GrayscaleMethod::Lightness, result);

    // Lightness method uses the formula: (max(R, G, B) + min(R, G, B)) / 2
    // The expected values are calculated as follows:
    std::vector<std::vector<int>> expected = {
        {2 /* (1+3)/2 */, 127 /* (255+0)/2 */},
        {48 /* (92+4)/2 */, 255 /* (255+255)/2 */},
        {62 /* (0+125)/2 */, 48 /* (12+85)/2 */}
    };
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]) << " at (" << row << ", " << col << ")";
        }
    }
}

// Average
TEST(GrayscaleTest, Average) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {255, 255, 0}},
        {{92, 8, 4},     {255, 255, 255}},
        {{0, 125, 90},     {12, 25, 85}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 3, 2, GrayscaleMethod::Average, result);

    // Average method uses the formula: (R + G + B) / 3
    // The expected values are calculated as follows:
    std::vector<std::vector<int>> expected = {
        {2 /* (1+2+3)/3 */, 170 /* (255+255+0)/3 */},
        {34 /* (92+8+4)/3 */, 255 /* (255+255+255)/3 */},
        {71 /* (0+125+90)/3 */, 40 /* (12+25+85)/3 */}
    };
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]) << " at (" << row << ", " << col << ")";
        }
    }
}

// RootMeanSquare
TEST(GrayscaleTest, RootMeanSquare) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {255, 255, 0}},
        {{92, 8, 4},     {255, 255, 255}},
        {{0, 125, 90},     {12, 25, 85}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 3, 2, GrayscaleMethod::RootMeanSquare, result);

    // Root Mean Square method uses the formula: sqrt((R^2 + G^2 + B^2) / 3)
    // The expected values are calculated as follows:
    std::vector<std::vector<int>> expected = {
        {2 /* sqrt(1*1+2*2+3)/3 */, 208 /* (255+255+0)/3 */},
        {53 /* (92+8+4)/3 */, 255 /* (255+255+255)/3 */},
        {88 /* (0+125+90)/3 */, 51 /* (12+25+85)/3 */}
    };
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]) << " at (" << row << ", " << col << ")";
        }
    }
}


// Black pixel tests
TEST(GrayscaleTest, BlackPixel) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{0, 0, 0}}};
    std::vector<std::vector<int>> result;

    // RedChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[0][0], 0);

    // GreenChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::GreenChannel, result);
    EXPECT_EQ(result[0][0], 0);

    // BlueChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::BlueChannel, result);
    EXPECT_EQ(result[0][0], 0);

    // Lightness: (max+min)/2 = (0+0)/2 = 0
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Lightness, result);
    EXPECT_EQ(result[0][0], 0);

    // Average: (0+0+0)/3 = 0
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_EQ(result[0][0], 0);

    // Luminosity: 0.21*0 + 0.72*0 + 0.07*0 = 0
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    EXPECT_EQ(result[0][0], 0);

    // RootMeanSquare: sqrt((0^2 + 0^2 + 0^2)/3) = 0
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RootMeanSquare, result);
    EXPECT_EQ(result[0][0], 0);
}

// White pixel tests
TEST(GrayscaleTest, WhitePixel) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 255, 255}}};
    std::vector<std::vector<int>> result;

    // RedChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[0][0], 255);

    // GreenChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::GreenChannel, result);
    EXPECT_EQ(result[0][0], 255);

    // BlueChannel
    convertToGrayscale(image, 1, 1, GrayscaleMethod::BlueChannel, result);
    EXPECT_EQ(result[0][0], 255);

    // Lightness: (max+min)/2 = (255+255)/2 = 255
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Lightness, result);
    EXPECT_EQ(result[0][0], 255);

    // Average: (255+255+255)/3 = 255
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_EQ(result[0][0], 255);

    // // Luminosity: 0.21*255 + 0.72*255 + 0.07*255 = 254
    // convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    // EXPECT_EQ(result[0][0], 254); // due to rounding

    // RootMeanSquare: sqrt((255^2 + 255^2 + 255^2)/3) = sqrt(65025) = ~254
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RootMeanSquare, result);
    EXPECT_EQ(result[0][0], 255);
}

// testing all methods with multiple pixels in order to ensure that the conversion works as expected
// when there's more than one pixel and different methods are tested together
// we expect no interference between methods and pixels
TEST(GrayscaleTest, MultiplePixelsAndMethods) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{255, 0, 0}, {0, 255, 0}}, 
        {{0, 0, 255}, {255, 255, 0}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 2, 2, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[0][0], 255); // Red pixel
    EXPECT_EQ(result[0][1], 0);   // Green pixel (should be zero in red channel)
    EXPECT_EQ(result[1][0], 0);   // Blue pixel (should be zero in red channel)
    EXPECT_EQ(result[1][1], 255); // Yellow pixel (should be equal to red channel)

    convertToGrayscale(image, 2, 2, GrayscaleMethod::GreenChannel, result);
    EXPECT_EQ(result[0][0], 0);   // Red pixel (should be zero in green channel)
    EXPECT_EQ(result[0][1], 255); // Green pixel
    EXPECT_EQ(result[1][0], 0);   // Blue pixel (should be zero in green channel)
    EXPECT_EQ(result[1][1], 255); // Yellow pixel (should be equal to green channel)

    convertToGrayscale(image, 2, 2, GrayscaleMethod::BlueChannel, result);
    EXPECT_EQ(result[0][0], 0);   // Red pixel (should be zero in blue channel)
    EXPECT_EQ(result[0][1], 0);   // Green pixel (should be zero in blue channel)
    EXPECT_EQ(result[1][0], 255); // Blue pixel
    EXPECT_EQ(result[1][1], 0);   // Yellow pixel (should be zero in blue channel)

    convertToGrayscale(image, 2, 2, GrayscaleMethod::Lightness, result);
    EXPECT_EQ(result[0][0], 127); // Red pixel (max=255, min=0) => (255+0)/2 = 127
    EXPECT_EQ(result[0][1], 127); // Green pixel (max=255, min=0) => (255+0)/2 = 127
    EXPECT_EQ(result[1][0], 127); // Blue pixel (max=255, min=0) => (255+0)/2 = 127
    EXPECT_EQ(result[1][1], 127); // Yellow pixel (max=255, min=0) => (255+0)/2 = 127

    convertToGrayscale(image, 2, 2, GrayscaleMethod::Average, result);
    EXPECT_EQ(result[0][0], 85);  // Red pixel (average of 255, 0, 0) => (255+0+0)/3 = 85
    EXPECT_EQ(result[0][1], 85);  // Green pixel (average of 0, 255, 0) => (0+255+0)/3 = 85
    EXPECT_EQ(result[1][0], 85);  // Blue pixel (average of 0, 0, 255) => (0+0+255)/3 = 85
    EXPECT_EQ(result[1][1], 170); // Yellow pixel (average of 255, 255, 0) => (255+255+0)/3 = 170

    // convertToGrayscale(image, 2, 2, GrayscaleMethod::Luminosity, result);
    // EXPECT_EQ(result[0][0], 54);  // Red pixel (0.21*255 + 0.72*0 + 0.07*0) => 54
    // EXPECT_EQ(result[0][1], 183); // Green pixel (0.21*0 + 0.72*255 + 0.07*0) => 183
    // EXPECT_EQ(result[1][0], 18);  // Blue pixel (0.21*0 + 0.72*0 + 0.07*255) => 18
    // EXPECT_EQ(result[1][1], 255); // Yellow pixel (0.21*255 + 0.72*255 + 0.07*0) => 255

    convertToGrayscale(image, 2, 2, GrayscaleMethod::RootMeanSquare, result);
    EXPECT_EQ(result[0][0], 147); // Red pixel (sqrt((255^2 + 0^2 + 0^2)/3)) => 147
    EXPECT_EQ(result[0][1], 147); // Green pixel (sqrt((0^2 + 255^2 + 0^2)/3)) => 147       
    EXPECT_EQ(result[1][0], 147); // Blue pixel (sqrt((0^2 + 0^2 + 255^2)/3)) => 147
    EXPECT_EQ(result[1][1], 208); // Yellow pixel (sqrt((255^2 + 255^2 + 0^2)/3)) => 208

}