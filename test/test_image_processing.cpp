#include "image_processing.hpp"
#include <gtest/gtest.h>

TEST(GrayscaleTest, RedChannelConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 0, 0}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[0][0], 255); // Red channel only
}

TEST(GrayscaleTest, LuminosityConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 255, 255}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    EXPECT_EQ(result[0][0], 254); // White pixel, should remain max gray
}

TEST(GrayscaleTest, AverageConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_EQ(result[0][0], (30 + 90 + 150) / 3); // Arithmetic mean
}

// new tests
TEST(GrayscaleTest, EmptyImage) {
    std::vector<std::vector<std::array<int, 3>>> image = {};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 0, 0, GrayscaleMethod::Average, result);
    EXPECT_EQ(result.size(), 0); 
}

TEST(GrayscaleTest, ImageSize) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_EQ(result.size(), 1); 
    EXPECT_EQ(result[0].size(), 1); 
}

TEST(GrayscaleTest, ImageSize2) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150},{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 2, GrayscaleMethod::Average, result);
    EXPECT_EQ(result.size(), 1); 
    EXPECT_EQ(result[0].size(), 2); 
}

TEST(GrayscaleTest, ImageSize3) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}},{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 2, 1, GrayscaleMethod::Average, result);
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
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][0]);
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
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][1]);
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
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], image[row][col][2]);
        }
    }
}

// Lightness
TEST(GrayscaleTest, Lightness) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{1, 2, 3},     {255, 255, 0}},
        {{92, 8, 4},     {255, 255, 255}},
        {{0, 125, 90},     {12, 25, 85}}
    };
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 3, 2, GrayscaleMethod::Lightness, result);

    std::vector<std::vector<int>> expected = {
        {2 /* (1+3)/2 */, 127 /* (255+0)/2 */},
        {48 /* (92+4)/2 */, 255 /* (255+255)/2 */},
        {62 /* (0+125)/2 */, 48 /* (12+85)/2 */}
    };
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]);
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

    std::vector<std::vector<int>> expected = {
        {2 /* (1+2+3)/3 */, 170 /* (255+255+0)/3 */},
        {34 /* (92+8+4)/3 */, 255 /* (255+255+255)/3 */},
        {71 /* (0+125+90)/3 */, 40 /* (12+25+85)/3 */}
    };
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]);
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

    std::vector<std::vector<int>> expected = {
        {2 /* sqrt(1*1+2*2+3)/3 */, 208 /* (255+255+0)/3 */},
        {53 /* (92+8+4)/3 */, 255 /* (255+255+255)/3 */},
        {88 /* (0+125+90)/3 */, 51 /* (12+25+85)/3 */}
    };
    
    for (int row = 0; row < image.size(); row++) {
        for (int col = 0; col < image[0].size(); col++) {
            EXPECT_EQ(result[row][col], expected[row][col]);
        }
    }
}
