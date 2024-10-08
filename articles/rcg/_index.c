/////////////////
/// ---
/// title: "Retro Computer Graphics - Introduction"
/// ---
///
/// Introduction
/// ---------------------------
///
/// This is the first article in a (hopefully) long series about old-school software rendering.
/// All the code presented will be written in C99, using SDL2 for graphics output (to make the resulting code more cross platform)
///
/// Prequesits
/// ---------------------------
///
/// What I expect you to know:
/// - Fixed point numbers: I'm generally not going to use any floating point numbers
///
/// Code style
/// ---------------------------
///
/// * I'm going to use 'RCG' as a prefix for functions/typedefs/etc. (standing for retro computer graphics). If the prefix is lowercase, the function/type is meant to be internal.
/// * I'm not
///
/// Assets
/// ---------------------------
///
/// The example code at the end of every article makes use of various assets from different sources. See [Appendix 0](/rcg/appendix0) for more information.
///
/// Compiling
/// ---------------------------
///
/// Code compilation depends on your choice of code organisation and target platform. Since this tutorial doesn't have any dependencies outside of SDL2, compiling should be very simple. However, in the individual articles source code, compilation instructions for linux/gcc can be found at the top of the file.
///
/// Showcase
/// ---------------------------
///
/// Source code?
/// ---------------------------
///
/// The source code, for both the articles themselves and the resulting programs can be found in this git repository: <https://github.com/Captain4LK/website_articles>. Additionally the articles themselves link to their respective source code.
///
/// ---------------------------
/// Article Series:
///   * retro computer graphics - Introduction
///   * Chapter 1 - Basic framework
///     * [Chapter 1.1 - Initial setup: graphics output and input](/rcg/chapter1_1)
///     * [Chapter 1.2 - Frame buffer and color palette](/rcg/chapter1_2)
///     * [Chapter 1.3 - Basic drawing](/rcg/chapter1_3)
///     * [Chapter 1.4 - Image loading and drawing](/rcg/chapter1_4)
///     * [Chapter 1.5 - Colormaps: lighting and transparency](/rcg/chapter1_5)
///     * [Chapter 1.6 - Fixed point math](/rcg/chapter1_6)
///   * Appendix
///     * [retro computer graphics - Appendix 0 - Assets](/rcg/appendix0)
///
