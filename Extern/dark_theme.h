/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  StyleAsCode exporter v1.2 - Style data exported as a values array  //
//                                                                     //
//  USAGE: On init call: GuiLoadStyleDarkTheme();                      //
//                                                                     //
//  more info and bugs-report:  github.com/raysan5/raygui              //
//  feedback and support:       ray[at]raylibtech.com                  //
//                                                                     //
//  Copyright (c) 2020-2022 raylib technologies (@raylibtech)          //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#define DEFAULT_STYLE_PROPS_COUNT  15

// Custom style name: default
static const GuiStyleProp darkThemeProps[DEFAULT_STYLE_PROPS_COUNT] =
{
	{ 0,  0, 0x7c7c7cff },    // DEFAULT_BORDER_COLOR_NORMAL
	{ 0,  1, 0x363636ff },    // DEFAULT_BASE_COLOR_NORMAL
	{ 0,  2, 0x979797ff },    // DEFAULT_TEXT_COLOR_NORMAL
	{ 0,  3, 0x4688a6ff },    // DEFAULT_BORDER_COLOR_FOCUSED
	{ 0,  4, 0x011f36ff },    // DEFAULT_BASE_COLOR_FOCUSED
	{ 0,  5, 0x4688a6ff },    // DEFAULT_TEXT_COLOR_FOCUSED
	{ 0,  6, 0x38cafbff },    // DEFAULT_BORDER_COLOR_PRESSED
	{ 0,  7, 0x004b68ff },    // DEFAULT_BASE_COLOR_PRESSED
	{ 0,  8, 0x38cafbff },    // DEFAULT_TEXT_COLOR_PRESSED
	{ 0,  9, 0x3d464aff },    // DEFAULT_BORDER_COLOR_DISABLED
	{ 0, 10, 0x161819ff },    // DEFAULT_BASE_COLOR_DISABLED
	{ 0, 11, 0x3d464aff },    // DEFAULT_TEXT_COLOR_DISABLED
	{ 0, 18, 0x3d484dff },    // DEFAULT_LINE_COLOR
	{ 0, 19, 0x0a0a0aff },    // DEFAULT_BACKGROUND_COLOR
	{ 4,  7, 0x38cafbff },    // SLIDER_BASE_COLOR_PRESSED
};

// Style loading function: DarkTheme
static void GuiLoadStyleDarkTheme(void)
{
	// Load style properties provided
	// NOTE: Default properties are propagated
	for (int i = 0; i < DEFAULT_STYLE_PROPS_COUNT; i++)
		GuiSetStyle(darkThemeProps[i].controlId, darkThemeProps[i].propertyId, darkThemeProps[i].propertyValue);

	// TODO: Custom user style setup: Set specific properties here (if required)
	// i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}