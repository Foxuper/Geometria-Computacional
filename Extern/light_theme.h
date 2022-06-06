/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  StyleAsCode exporter v1.2 - Style data exported as a values array  //
//                                                                     //
//  USAGE: On init call: GuiLoadStyleLightTheme();                     //
//                                                                     //
//  more info and bugs-report:  github.com/raysan5/raygui              //
//  feedback and support:       ray[at]raylibtech.com                  //
//                                                                     //
//  Copyright (c) 2020-2022 raylib technologies (@raylibtech)          //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#define DEFAULT_STYLE_PROPS_COUNT  14

// Custom style name: default
static const GuiStyleProp lightThemProps[DEFAULT_STYLE_PROPS_COUNT] =
{
	{ 0,  0, 0x838383ff },    // DEFAULT_BORDER_COLOR_NORMAL
	{ 0,  1, 0xc9c9c9ff },    // DEFAULT_BASE_COLOR_NORMAL
	{ 0,  2, 0x686868ff },    // DEFAULT_TEXT_COLOR_NORMAL
	{ 0,  3, 0x5bb2d9ff },    // DEFAULT_BORDER_COLOR_FOCUSED
	{ 0,  4, 0xc9effeff },    // DEFAULT_BASE_COLOR_FOCUSED
	{ 0,  5, 0x6c9bbcff },    // DEFAULT_TEXT_COLOR_FOCUSED
	{ 0,  6, 0x0492c7ff },    // DEFAULT_BORDER_COLOR_PRESSED
	{ 0,  7, 0x97e8ffff },    // DEFAULT_BASE_COLOR_PRESSED
	{ 0,  8, 0x368bafff },    // DEFAULT_TEXT_COLOR_PRESSED
	{ 0,  9, 0xb5c1c2ff },    // DEFAULT_BORDER_COLOR_DISABLED
	{ 0, 10, 0xe6e9e9ff },    // DEFAULT_BASE_COLOR_DISABLED
	{ 0, 11, 0xaeb7b8ff },    // DEFAULT_TEXT_COLOR_DISABLED
	{ 0, 18, 0x90abb5ff },    // DEFAULT_LINE_COLOR
	{ 0, 19, 0xf5f5f5ff },    // DEFAULT_BACKGROUND_COLOR
};

// Style loading function: LightTheme
static void GuiLoadStyleLightTheme(void)
{
	// Load style properties provided
	// NOTE: Default properties are propagated
	for (int i = 0; i < DEFAULT_STYLE_PROPS_COUNT; i++)
		GuiSetStyle(lightThemProps[i].controlId, lightThemProps[i].propertyId, lightThemProps[i].propertyValue);

	// TODO: Custom user style setup: Set specific properties here (if required)
	// i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}