#include <JuceHeader.h>
#include <array>
#include <optional>
#include "Engine.h"
#include "Sequencer.h"

namespace rb338
{
    // =========================================================================
    // Theme System - Supports Light and Dark modes
    // =========================================================================
    struct Theme
    {
        juce::Colour beige, panelBg, mainGrey, darkGrey;
        juce::Colour orange, orangeDark, idleGrey, footerGrey;
        juce::Colour borderLight, headerBorder;
        juce::Colour stepWhiteTop, stepWhiteBot, stepOrgTop, stepOrgBot;
        juce::Colour stepBlkTop, stepBlkBot;
        juce::Colour ledOff, ledOn;
        juce::Colour lcdBg, lcdBorder, lcdBright, lcdDim;
        juce::Colour gridLine, gridLabelBg, cellActive, cellAccent;
        juce::Colour textDark, textMid, textLight, textWhite;

        static Theme light()
        {
            Theme t;
            t.beige = juce::Colour(0xffd8d8d2); t.panelBg = juce::Colour(0xffe5e5df);
            t.mainGrey = juce::Colour(0xffc8c8c0); t.darkGrey = juce::Colour(0xff4a4a4a);
            t.orange = juce::Colour(0xffff6a00); t.orangeDark = juce::Colour(0xffb34a00);
            t.idleGrey = juce::Colour(0xff2a2a2a); t.footerGrey = juce::Colour(0xffb0b0a8);
            t.borderLight = juce::Colour(0xffb8b8b0); t.headerBorder = juce::Colour(0xffa8a8a0);
            t.stepWhiteTop = juce::Colour(0xffeeeeee); t.stepWhiteBot = juce::Colour(0xffcccccc);
            t.stepOrgTop = juce::Colour(0xffff9d5c); t.stepOrgBot = juce::Colour(0xffff6a00);
            t.stepBlkTop = juce::Colour(0xff555555); t.stepBlkBot = juce::Colour(0xff222222);
            t.ledOff = juce::Colour(0xff444444); t.ledOn = juce::Colour(0xffff3300);
            t.lcdBg = juce::Colour(0xff1a1a1a); t.lcdBorder = juce::Colour(0xff333333);
            t.lcdBright = juce::Colour(0xff33cc33); t.lcdDim = juce::Colour(0xff116611);
            t.gridLine = juce::Colour(0xffb8b8b0); t.gridLabelBg = juce::Colour(0xffd0d0c8);
            t.cellActive = juce::Colour(0xffff6a00); t.cellAccent = juce::Colour(0xffff3300);
            t.textDark = juce::Colour(0xff333333); t.textMid = juce::Colour(0xff555555);
            t.textLight = juce::Colour(0xff777777); t.textWhite = juce::Colour(0xffffffff);
            return t;
        }

        static Theme dark()
        {
            Theme t;
            t.beige = juce::Colour(0xff2a2a2a); t.panelBg = juce::Colour(0xff1e1e1e);
            t.mainGrey = juce::Colour(0xff3a3a3a); t.darkGrey = juce::Colour(0xff666666);
            t.orange = juce::Colour(0xffff8833); t.orangeDark = juce::Colour(0xffcc6622);
            t.idleGrey = juce::Colour(0xff1a1a1a); t.footerGrey = juce::Colour(0xff333333);
            t.borderLight = juce::Colour(0xff555555); t.headerBorder = juce::Colour(0xff444444);
            t.stepWhiteTop = juce::Colour(0xff666666); t.stepWhiteBot = juce::Colour(0xff444444);
            t.stepOrgTop = juce::Colour(0xffff9d5c); t.stepOrgBot = juce::Colour(0xffff6a00);
            t.stepBlkTop = juce::Colour(0xff777777); t.stepBlkBot = juce::Colour(0xff555555);
            t.ledOff = juce::Colour(0xff222222); t.ledOn = juce::Colour(0xffff6633);
            t.lcdBg = juce::Colour(0xff0a0a0a); t.lcdBorder = juce::Colour(0xff222222);
            t.lcdBright = juce::Colour(0xff33cc33); t.lcdDim = juce::Colour(0xff116611);
            t.gridLine = juce::Colour(0xff555555); t.gridLabelBg = juce::Colour(0xff444444);
            t.cellActive = juce::Colour(0xffff8833); t.cellAccent = juce::Colour(0xffff6633);
            t.textDark = juce::Colour(0xffcccccc); t.textMid = juce::Colour(0xffaaaaaa);
            t.textLight = juce::Colour(0xff888888); t.textWhite = juce::Colour(0xffffffff);
            return t;
        }
    };

    // Convenience namespace - using global theme
    namespace Clr
    {
        static juce::Colour beige, panelBg, mainGrey, darkGrey;
        static juce::Colour orange, orangeDark, idleGrey, footerGrey;
        static juce::Colour borderLight, headerBorder;
        static juce::Colour stepWhiteTop, stepWhiteBot, stepOrgTop, stepOrgBot;
        static juce::Colour stepBlkTop, stepBlkBot;
        static juce::Colour ledOff, ledOn;
        static juce::Colour lcdBg, lcdBorder, lcdBright, lcdDim;
        static juce::Colour gridLine, gridLabelBg, cellActive, cellAccent;
        static juce::Colour textDark, textMid, textLight, textWhite;
    }

    static void applyTheme(const Theme& theme)
    {
        Clr::beige = theme.beige; Clr::panelBg = theme.panelBg;
        Clr::mainGrey = theme.mainGrey; Clr::darkGrey = theme.darkGrey;
        Clr::orange = theme.orange; Clr::orangeDark = theme.orangeDark;
        Clr::idleGrey = theme.idleGrey; Clr::footerGrey = theme.footerGrey;
        Clr::borderLight = theme.borderLight; Clr::headerBorder = theme.headerBorder;
        Clr::stepWhiteTop = theme.stepWhiteTop; Clr::stepWhiteBot = theme.stepWhiteBot;
        Clr::stepOrgTop = theme.stepOrgTop; Clr::stepOrgBot = theme.stepOrgBot;
        Clr::stepBlkTop = theme.stepBlkTop; Clr::stepBlkBot = theme.stepBlkBot;
        Clr::ledOff = theme.ledOff; Clr::ledOn = theme.ledOn;
        Clr::lcdBg = theme.lcdBg; Clr::lcdBorder = theme.lcdBorder;
        Clr::lcdBright = theme.lcdBright; Clr::lcdDim = theme.lcdDim;
        Clr::gridLine = theme.gridLine; Clr::gridLabelBg = theme.gridLabelBg;
        Clr::cellActive = theme.cellActive; Clr::cellAccent = theme.cellAccent;
        Clr::textDark = theme.textDark; Clr::textMid = theme.textMid;
        Clr::textLight = theme.textLight; Clr::textWhite = theme.textWhite;
    }

    // Initialize with light theme
    static struct ThemeInitializer {
        ThemeInitializer() { applyTheme(Theme::light()); }
    } themeInit;

    // LCD Color Presets
    enum class LCDColor { Green, Red, Amber, Blue, Cyan, White };

    static void setLCDColor(LCDColor color)
    {
        switch (color)
        {
            case LCDColor::Green:
                Clr::lcdBright = juce::Colour(0xff33cc33);
                Clr::lcdDim    = juce::Colour(0xff116611);
                break;
            case LCDColor::Red:
                Clr::lcdBright = juce::Colour(0xffcc3333);
                Clr::lcdDim    = juce::Colour(0xff661111);
                break;
            case LCDColor::Amber:
                Clr::lcdBright = juce::Colour(0xffffaa00);
                Clr::lcdDim    = juce::Colour(0xff885500);
                break;
            case LCDColor::Blue:
                Clr::lcdBright = juce::Colour(0xff3399ff);
                Clr::lcdDim    = juce::Colour(0xff114488);
                break;
            case LCDColor::Cyan:
                Clr::lcdBright = juce::Colour(0xff00ffff);
                Clr::lcdDim    = juce::Colour(0xff008888);
                break;
            case LCDColor::White:
                Clr::lcdBright = juce::Colour(0xffffffff);
                Clr::lcdDim    = juce::Colour(0xff888888);
                break;
        }
    }

    // =========================================================================
    // Grid row definitions
    // =========================================================================
    struct GridRowDef { Instrument instrument; const char* label; };
    static const GridRowDef gridRows[] = {
        { Instrument::Kick,      "BD" },
        { Instrument::Snare,     "SD" },
        { Instrument::TomLow,    "LT" },
        { Instrument::TomMid,    "MT" },
        { Instrument::TomHigh,   "HT" },
        { Instrument::Rim,       "RS" },
        { Instrument::Clap,      "CP" },
        { Instrument::ClosedHat, "CH" },
        { Instrument::OpenHat,   "OH" },
        { Instrument::Crash,     "CR" },
        { Instrument::Ride,      "RD" },
    };
    static constexpr int numGridRows = 11;
    static constexpr int numBanks = 2;
    static constexpr int numPatternsPerBank = 16;

    struct PatternData
    {
        StepState steps[(int)Instrument::Count][16] {};
        bool automationActive[(int)Instrument::Count][(int)AutomationParam::Count][16] {};
        float automationValue[(int)Instrument::Count][(int)AutomationParam::Count][16] {};
        float bpm = 120.0f;
        float shuffle = 0.0f;
        float accent = 0.5f;
        juce::String name;
    };

    // =========================================================================
    // Knob definition for instrument sections
    // =========================================================================
    enum class ParamType { Level, Tune, Decay, Tone, Snappy };

    struct KnobDef
    {
        const char* label;
        Instrument  instrument;
        ParamType   param;
    };

    struct SectionDef
    {
        const char* title;
        KnobDef     knobs[4];
        int         numKnobs;
        bool        drawRightBorder;
    };

    static const SectionDef sectionDefs[] = {
        { "Bass Drum", {
            { "Tune",   Instrument::Kick, ParamType::Tune },
            { "Level",  Instrument::Kick, ParamType::Level },
            { "Attack", Instrument::Kick, ParamType::Tone },
            { "Decay",  Instrument::Kick, ParamType::Decay }
        }, 4, true },
        { "Snare Drum", {
            { "Tune",   Instrument::Snare, ParamType::Tune },
            { "Level",  Instrument::Snare, ParamType::Level },
            { "Tone",   Instrument::Snare, ParamType::Tone },
            { "Snappy", Instrument::Snare, ParamType::Snappy }
        }, 4, true },
        { "Low Tom", {
            { "Tune",  Instrument::TomLow, ParamType::Tune },
            { "Level", Instrument::TomLow, ParamType::Level },
            { "Decay", Instrument::TomLow, ParamType::Decay },
            { "", Instrument::Kick, ParamType::Level }
        }, 3, true },
        { "Mid Tom", {
            { "Tune",  Instrument::TomMid, ParamType::Tune },
            { "Level", Instrument::TomMid, ParamType::Level },
            { "Decay", Instrument::TomMid, ParamType::Decay },
            { "", Instrument::Kick, ParamType::Level }
        }, 3, true },
        { "Hi Tom", {
            { "Tune",  Instrument::TomHigh, ParamType::Tune },
            { "Level", Instrument::TomHigh, ParamType::Level },
            { "Decay", Instrument::TomHigh, ParamType::Decay },
            { "", Instrument::Kick, ParamType::Level }
        }, 3, true },
        { "Rim", {
            { "Level", Instrument::Rim, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level }
        }, 1, true },
        { "Clap", {
            { "Level", Instrument::Clap, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level }
        }, 1, true },
        { "Hi Hat", {
            { "CH Lvl", Instrument::ClosedHat, ParamType::Level },
            { "OH Lvl", Instrument::OpenHat,   ParamType::Level },
            { "CH Dec", Instrument::ClosedHat, ParamType::Decay },
            { "OH Dec", Instrument::OpenHat,   ParamType::Decay }
        }, 4, true },
        { "Cymbal", {
            { "Crash L", Instrument::Crash, ParamType::Level },
            { "Ride L",  Instrument::Ride,  ParamType::Level },
            { "Crash T", Instrument::Crash, ParamType::Tune },
            { "Ride T",  Instrument::Ride,  ParamType::Tune }
        }, 4, false }
    };
    static constexpr int numSections = 9;

    static AutomationParam toAutomationParam(ParamType param)
    {
        switch (param)
        {
            case ParamType::Level:  return AutomationParam::Level;
            case ParamType::Tune:   return AutomationParam::Tune;
            case ParamType::Decay:  return AutomationParam::Decay;
            case ParamType::Tone:   return AutomationParam::Tone;
            case ParamType::Snappy: return AutomationParam::Snappy;
        }

        return AutomationParam::Level;
    }

    // =========================================================================
    // LookAndFeel
    // =========================================================================
    class LookAndFeel909 : public juce::LookAndFeel_V4
    {
    public:
        LookAndFeel909()
        {
            setColour(juce::ResizableWindow::backgroundColourId, Clr::mainGrey);
        }

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                              float sliderPos, float startAngle, float endAngle,
                              juce::Slider&) override
        {
            auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(2.0f);
            auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
            auto cx = bounds.getCentreX();
            auto cy = bounds.getCentreY();
            auto angle = startAngle + sliderPos * (endAngle - startAngle);

            // Drop shadow
            g.setColour(juce::Colours::black.withAlpha(0.35f));
            g.fillEllipse(cx - radius, cy - radius + 3.0f, radius * 2.0f, radius * 2.0f);

            // Knob body: radial gradient
            juce::ColourGradient grad(juce::Colour(0xff666666), cx - radius * 0.4f, cy - radius * 0.4f,
                                      juce::Colour(0xff000000), cx + radius, cy + radius, true);
            grad.addColour(0.7, juce::Colour(0xff222222));
            g.setGradientFill(grad);
            g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

            // Inner highlight
            g.setColour(juce::Colours::white.withAlpha(0.12f));
            g.fillEllipse(cx - radius * 0.5f, cy - radius * 0.7f, radius, radius * 0.5f);

            // Orange pointer
            juce::Path pointer;
            auto pLen = radius * 0.6f;
            pointer.addRoundedRectangle(-1.5f, -pLen, 3.0f, pLen, 1.5f);
            g.setColour(Clr::orange);
            g.fillPath(pointer, juce::AffineTransform::rotation(angle).translated(cx, cy));
        }

        void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                  const juce::Colour& bgColour, bool hover, bool down) override
        {
            auto b = button.getLocalBounds().toFloat();
            auto col = bgColour;
            if (down) col = col.darker(0.15f);
            else if (hover) col = col.brighter(0.08f);

            // Shadow
            g.setColour(juce::Colours::black.withAlpha(0.25f));
            g.fillRoundedRectangle(b.translated(0, 1), 2.0f);

            g.setColour(col);
            g.fillRoundedRectangle(b, 2.0f);

            g.setColour(col.brighter(0.2f));
            g.fillRoundedRectangle(b.removeFromTop(b.getHeight() * 0.4f).reduced(1), 1.0f);
        }
    };

    // =========================================================================
    // Settings Panel
    // =========================================================================
    class SettingsPanel : public juce::Component
    {
    public:
        std::function<void()> onThemeChanged;
        std::function<void()> onClose;

        SettingsPanel()
        {
            addAndMakeVisible(titleLabel);
            titleLabel.setText("SETTINGS", juce::dontSendNotification);
            titleLabel.setFont(juce::Font(14.0f, juce::Font::bold));
            titleLabel.setJustificationType(juce::Justification::centred);

            // Dark mode toggle
            addAndMakeVisible(darkModeLabel);
            darkModeLabel.setText("Dark Mode:", juce::dontSendNotification);
            darkModeLabel.setFont(juce::Font(12.0f, juce::Font::plain));

            addAndMakeVisible(darkModeToggle);
            darkModeToggle.setButtonText("OFF");
            darkModeToggle.onClick = [this]() {
                bool isDark = darkModeToggle.getToggleState();
                darkModeToggle.setButtonText(isDark ? "ON" : "OFF");
                applyTheme(isDark ? Theme::dark() : Theme::light());
                if (onThemeChanged) onThemeChanged();
            };

            // LCD Color selector
            addAndMakeVisible(lcdColorLabel);
            lcdColorLabel.setText("LCD Color:", juce::dontSendNotification);
            lcdColorLabel.setFont(juce::Font(12.0f, juce::Font::plain));

            const char* colors[] = { "Green", "Red", "Amber", "Blue", "Cyan", "White" };
            for (int i = 0; i < 6; ++i)
            {
                auto* btn = new juce::TextButton(colors[i]);
                btn->onClick = [this, i]() {
                    setLCDColor((LCDColor)i);
                    if (onThemeChanged) onThemeChanged();
                };
                addAndMakeVisible(btn);
                colorButtons.add(btn);
            }

            // Close button
            addAndMakeVisible(closeButton);
            closeButton.setButtonText("CLOSE");
            closeButton.onClick = [this]() { if (onClose) onClose(); };
        }

        void paint(juce::Graphics& g) override
        {
            // Semi-transparent overlay
            g.fillAll(juce::Colours::black.withAlpha(0.8f));

            // Panel background
            auto bounds = getLocalBounds().reduced(100, 80);
            g.setColour(Clr::mainGrey);
            g.fillRoundedRectangle(bounds.toFloat(), 8.0f);
            g.setColour(Clr::borderLight);
            g.drawRoundedRectangle(bounds.toFloat(), 8.0f, 2.0f);
        }

        void resized() override
        {
            auto bounds = getLocalBounds().reduced(100, 80);
            auto area = bounds.reduced(20);

            titleLabel.setBounds(area.removeFromTop(30));
            area.removeFromTop(10);

            // Dark mode row
            auto darkRow = area.removeFromTop(30);
            darkModeLabel.setBounds(darkRow.removeFromLeft(100));
            darkModeToggle.setBounds(darkRow.removeFromLeft(80).reduced(0, 2));
            area.removeFromTop(15);

            // LCD color row
            auto lcdRow = area.removeFromTop(30);
            lcdColorLabel.setBounds(lcdRow.removeFromLeft(100));
            area.removeFromTop(10);

            // Color buttons (2 rows of 3)
            for (int row = 0; row < 2; ++row)
            {
                auto btnRow = area.removeFromTop(35);
                btnRow.removeFromLeft(20);
                for (int col = 0; col < 3; ++col)
                {
                    int idx = row * 3 + col;
                    colorButtons[idx]->setBounds(btnRow.removeFromLeft(90).reduced(2));
                    btnRow.removeFromLeft(5);
                }
                area.removeFromTop(5);
            }

            area.removeFromTop(20);
            closeButton.setBounds(area.removeFromTop(40).reduced(bounds.getWidth() / 3, 0));
        }

    private:
        juce::Label titleLabel, darkModeLabel, lcdColorLabel;
        juce::ToggleButton darkModeToggle;
        juce::OwnedArray<juce::TextButton> colorButtons;
        juce::TextButton closeButton;
    };

    // =========================================================================
    // LCD Display with Shuffle & Accent Controls
    // =========================================================================
    class LCDDisplay : public juce::Component, private juce::Slider::Listener
    {
    public:
        std::function<void()> onRequestPatternManager;
        std::function<void(float)> onBpmChanged;

        LCDDisplay(Engine& e) : engine(e)
        {
            // Shuffle knob
            shuffleSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            shuffleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            shuffleSlider.setRange(0.0, 1.0, 0.01);
            shuffleSlider.setValue(0.0);
            shuffleSlider.addListener(this);
            shuffleSlider.setDoubleClickReturnValue(true, 0.0);
            addAndMakeVisible(shuffleSlider);

            // Accent level knob
            accentSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            accentSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            accentSlider.setRange(0.0, 1.0, 0.01);
            accentSlider.setValue(0.5); // Default 50% accent boost
            accentSlider.addListener(this);
            accentSlider.setDoubleClickReturnValue(true, 0.5);
            addAndMakeVisible(accentSlider);
        }

        void setPatternDisplay(int bank, int pattern, const juce::String& nm)
        {
            bankIndex = juce::jlimit(0, numBanks - 1, bank);
            patternIndex = juce::jlimit(0, numPatternsPerBank - 1, pattern);
            patternName = nm;
            repaint();
        }

        void setBpmValue(float bpm, bool notifyEngine)
        {
            const float next = juce::jlimit(60.0f, 180.0f, bpm);
            if (next == currentBpm)
                return;

            currentBpm = next;
            if (notifyEngine)
                engine.setBpm(currentBpm);
            if (onBpmChanged)
                onBpmChanged(currentBpm);
            repaint();
        }

        void setShuffleAccent(float shuffle, float accent)
        {
            shuffleSlider.setValue(shuffle, juce::dontSendNotification);
            accentSlider.setValue(accent, juce::dontSendNotification);
        }

        void paint(juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat();
            auto controlsArea = b.removeFromRight(120); // Space for both knobs
            auto accentArea = controlsArea.removeFromRight(60);
            auto shuffleArea = controlsArea;

            g.setColour(Clr::lcdBg);
            g.fillRoundedRectangle(b, 6.0f);
            g.setColour(Clr::lcdBorder);
            g.drawRoundedRectangle(b, 6.0f, 2.0f);

            // Inset shadow
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillRoundedRectangle(b.reduced(2).removeFromTop(8), 4.0f);

            auto inner = b.reduced(10, 6);

            // Top row
            auto topRow = inner.removeFromTop(14);
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 10.0f, juce::Font::plain));
            g.setColour(Clr::lcdDim);
            g.drawText("PATTERN", topRow, juce::Justification::centredLeft, false);
            g.drawText(juce::String(juce::CharPointer_UTF8("\xe2\x97\x8f")) + " SYNC", topRow, juce::Justification::centredRight, false);

            inner.removeFromTop(2);

            // Bottom row (extra spacing between pattern ID and BPM for readability).
            g.setColour(Clr::lcdBright);
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 24.0f, juce::Font::bold));
            auto patternArea = inner.removeFromLeft(108);
            inner.removeFromLeft(14);
            auto bpmArea = inner;

            const juce::String id = juce::String(patternIndex + 1).paddedLeft('0', 2)
                + "-"
                + juce::String::charToString((juce_wchar)('A' + bankIndex));
            g.drawText(id, patternArea, juce::Justification::centredLeft, false);

            float bpm = currentBpm;
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 28.0f, juce::Font::bold));
            g.drawText(juce::String(bpm, 2), bpmArea, juce::Justification::centredRight, false);

            // Control labels
            g.setColour(Clr::textDark);
            g.setFont(juce::Font(7.0f, juce::Font::bold));
            g.drawText("SHUFFLE", shuffleArea.getX(), shuffleArea.getBottom() - 10, 60, 10,
                       juce::Justification::centred, false);
            g.drawText("ACCENT", accentArea.getX(), accentArea.getBottom() - 10, 60, 10,
                       juce::Justification::centred, false);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            auto controlsArea = area.removeFromRight(120);
            auto accentArea = controlsArea.removeFromRight(60);
            auto shuffleArea = controlsArea;

            shuffleSlider.setBounds(shuffleArea.reduced(10, 8).removeFromTop(38));
            accentSlider.setBounds(accentArea.reduced(10, 8).removeFromTop(38));
        }

        void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override
        {
            // Only handle wheel on LCD area, not control knobs (120px on right)
            if (e.x < getWidth() - 120)
            {
                setBpmValue(currentBpm + wheel.deltaY * 2.0f, true);
            }
        }

        void mouseDrag(const juce::MouseEvent& e) override
        {
            // Only handle drag on LCD area, not control knobs (120px on right)
            if (e.getMouseDownX() < getWidth() - 120)
            {
                auto delta = (float)-e.getDistanceFromDragStartY() * 0.2f;
                auto newBpm = juce::jlimit(60.0f, 180.0f, dragStartBpm + delta);
                if (newBpm != currentBpm)
                {
                    setBpmValue(newBpm, true);
                }
            }
        }

        void mouseDown(const juce::MouseEvent& e) override
        {
            if (e.mods.isPopupMenu() && e.x < getWidth() - 120)
            {
                if (onRequestPatternManager)
                    onRequestPatternManager();
                return;
            }
            dragStartBpm = currentBpm;
        }

        float currentBpm = 120.0f;

    private:
        Engine& engine;
        float dragStartBpm = 120.0f;
        juce::Slider shuffleSlider;
        juce::Slider accentSlider;
        int bankIndex = 0;
        int patternIndex = 0;
        juce::String patternName;

        void sliderValueChanged(juce::Slider* slider) override
        {
            if (slider == &shuffleSlider)
            {
                engine.getSequencer().setShuffle((float)slider->getValue());
            }
            else if (slider == &accentSlider)
            {
                engine.setAccentLevel((float)slider->getValue());
            }
        }
    };

    // =========================================================================
    // Instrument Section (header knob group)
    // =========================================================================
    class InstrumentSection : public juce::Component, private juce::Slider::Listener
    {
    public:
        InstrumentSection(Engine& e, const SectionDef& def, std::function<void(const KnobDef&, float)> onKnobMoved)
            : engine(e), sectionDef(def), onKnobMovedCb(std::move(onKnobMoved))
        {
            for (int i = 0; i < def.numKnobs; ++i)
            {
                auto* knob = new juce::Slider();
                knob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
                knob->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
                knob->setRange(0.0, 1.0, 0.01);
                const float initialValue = getInitialValue(def.knobs[i]);
                knob->setValue(initialValue);
                knob->addListener(this);
                knob->setDoubleClickReturnValue(true, initialValue);
                addAndMakeVisible(knob);
                knobs.add(knob);
            }
        }

        void paint(juce::Graphics& g) override
        {
            auto b = getLocalBounds();

            // Title
            g.setColour(Clr::textDark);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText(juce::String(sectionDef.title).toUpperCase(), b.removeFromTop(16).reduced(2, 0),
                       juce::Justification::centred, false);

            // Knob labels
            g.setFont(juce::Font(7.0f, juce::Font::bold));
            for (int i = 0; i < sectionDef.numKnobs; ++i)
            {
                if (knobs[i] != nullptr)
                {
                    auto kb = knobs[i]->getBounds();
                    g.drawText(juce::String(sectionDef.knobs[i].label).toUpperCase(),
                               kb.getX() - 1, kb.getBottom() + 2, kb.getWidth() + 2, 11,
                               juce::Justification::centred, false);
                }
            }

            // Right border
            if (sectionDef.drawRightBorder)
            {
                g.setColour(Clr::borderLight);
                g.drawLine((float)getWidth() - 1.0f, 0.0f, (float)getWidth() - 1.0f, (float)getHeight(), 1.0f);
            }
        }

        void resized() override
        {
            auto b = getLocalBounds();
            b.removeFromTop(18);
            b = b.reduced(4, 0);

            int cols = sectionDef.numKnobs > 1 ? 2 : 1;
            int knobSize = (sectionDef.numKnobs > 1) ? 30 : 34;
            int vGap = 14;
            int hGap = (b.getWidth() - cols * knobSize) / (cols + 1);
            hGap = juce::jmax(2, hGap);

            for (int i = 0; i < sectionDef.numKnobs; ++i)
            {
                int col = i % cols;
                int row = i / cols;
                int xPos = b.getX() + hGap + col * (knobSize + hGap);
                int yPos = b.getY() + row * (knobSize + vGap);
                knobs[i]->setBounds(xPos, yPos, knobSize, knobSize);
            }
        }

    private:
        Engine& engine;
        const SectionDef& sectionDef;
        juce::OwnedArray<juce::Slider> knobs;
        std::function<void(const KnobDef&, float)> onKnobMovedCb;

        float getInitialValue(const KnobDef& kd)
        {
            auto& ch = engine.getChannel(kd.instrument);
            switch (kd.param)
            {
                case ParamType::Level:  return ch.level;
                case ParamType::Tune:   return ch.params.tune;
                case ParamType::Decay:  return ch.params.decay;
                case ParamType::Tone:   return ch.params.tone;
                case ParamType::Snappy: return ch.params.snappy;
            }
            return 0.5f;
        }

        void sliderValueChanged(juce::Slider* slider) override
        {
            for (int i = 0; i < sectionDef.numKnobs; ++i)
            {
                if (knobs[i] == slider)
                {
                    auto& kd = sectionDef.knobs[i];
                    auto& ch = engine.getChannel(kd.instrument);
                    float val = (float)slider->getValue();
                    bool needResynth = false;

                    switch (kd.param)
                    {
                        case ParamType::Level:  ch.level = val; break;
                        case ParamType::Tune:
                            ch.params.tune = val;
                            needResynth = true;
                            break;
                        case ParamType::Decay:
                            ch.params.decay = val;
                            needResynth = true;
                            break;
                        case ParamType::Tone:
                            ch.params.tone = val;
                            needResynth = true;
                            break;
                        case ParamType::Snappy: ch.params.snappy = val; needResynth = true; break;
                    }

                    if (needResynth)
                        engine.updateInstrumentSound(kd.instrument);

                    if (onKnobMovedCb)
                        onKnobMovedCb(kd, val);
                    return;
                }
            }
        }
    };

    // =========================================================================
    // Sequencer Grid (expanded view)
    // =========================================================================
    class SequencerGrid : public juce::Component
    {
    public:
        SequencerGrid(Engine& e,
                      std::function<void(Instrument)> onSelectInst,
                      std::function<void(Instrument)> onClearAutomation)
            : engine(e),
              selectInstrumentCb(std::move(onSelectInst)),
              clearAutomationCb(std::move(onClearAutomation)) {}

        static constexpr int rowH = 24;  // 80% of original 30px
        static constexpr int labelW = 58;
        static constexpr int cellW = 49;

        int getDesiredHeight() const { return rowH * numGridRows; }

        void setCurrentStep(int step) { currentStep = step; }

        void paint(juce::Graphics& g) override
        {
            for (int row = 0; row < numGridRows; ++row)
            {
                int y = row * rowH;
                auto inst = gridRows[row].instrument;

                // Row background (alternating subtle shade)
                g.setColour(row % 2 == 0 ? Clr::mainGrey : Clr::mainGrey.darker(0.03f));
                g.fillRect(labelW, y, getWidth() - labelW, rowH);

                // Label
                g.setColour(Clr::gridLabelBg);
                g.fillRect(0, y, labelW, rowH);
                g.setColour(Clr::textDark);
                g.setFont(juce::Font(10.0f, juce::Font::bold));
                g.drawText(gridRows[row].label, 2, y, 24, rowH, juce::Justification::centred, false);

                const bool hasAutomation = engine.getSequencer().hasAutomation(inst);
                auto nilBounds = getNilBoundsForRow(row).toFloat();
                g.setColour(hasAutomation ? Clr::orange : juce::Colour(0xff777777));
                g.fillRoundedRectangle(nilBounds, 2.0f);
                g.setColour(hasAutomation ? Clr::textWhite : juce::Colour(0xffbbbbbb));
                g.setFont(juce::Font(8.0f, juce::Font::bold));
                g.drawText("NIL", nilBounds, juce::Justification::centred, false);

                // Cells
                for (int col = 0; col < 16; ++col)
                {
                    int cx = labelW + col * cellW;
                    auto state = engine.getSequencer().getStep(inst, col);

                    if (state == StepState::On)
                    {
                        g.setColour(Clr::cellActive);
                        g.fillRect(cx + 1, y + 1, cellW - 2, rowH - 2);
                        g.setColour(juce::Colours::white.withAlpha(0.25f));
                        g.fillRect(cx + 3, y + 3, cellW - 6, (rowH - 6) / 2);
                    }
                    else if (state == StepState::Accent)
                    {
                        g.setColour(Clr::cellAccent);
                        g.fillRect(cx + 1, y + 1, cellW - 2, rowH - 2);
                    }

                    // Current step highlight
                    if (col == currentStep && engine.isRunning())
                    {
                        g.setColour(juce::Colours::white.withAlpha(0.12f));
                        g.fillRect(cx, y, cellW, rowH);
                    }
                }

                // Grid lines
                g.setColour(Clr::gridLine);
                g.drawLine(0.0f, (float)(y + rowH), (float)getWidth(), (float)(y + rowH), 0.5f);
            }

            // Vertical grid lines
            g.setColour(Clr::gridLine);
            for (int col = 0; col <= 16; ++col)
                g.drawLine((float)(labelW + col * cellW), 0.0f, (float)(labelW + col * cellW), (float)getHeight(), 0.5f);

            // Label column right border
            g.drawLine((float)labelW, 0.0f, (float)labelW, (float)getHeight(), 1.0f);
        }

        void mouseDown(const juce::MouseEvent& e) override
        {
            int row = e.y / rowH;
            int col = (e.x - labelW) / cellW;

            if (row < 0 || row >= numGridRows) return;

            // Click on label.
            if (e.x < labelW)
            {
                auto inst = gridRows[row].instrument;
                if (getNilBoundsForRow(row).contains(e.getPosition()))
                {
                    if (clearAutomationCb && engine.getSequencer().hasAutomation(inst))
                        clearAutomationCb(inst);
                    return;
                }

                if (selectInstrumentCb)
                    selectInstrumentCb(inst);
                return;
            }

            if (col >= 0 && col < 16)
            {
                dragInstrument = gridRows[row].instrument;
                dragLastCol = col;

                // Off -> On -> Accent -> Off
                auto currentState = engine.getSequencer().getStep(dragInstrument, col);
                if (currentState == StepState::Off)      dragPaintState = StepState::On;
                else if (currentState == StepState::On)  dragPaintState = StepState::Accent;
                else                                      dragPaintState = StepState::Off;

                engine.getSequencer().setStep(dragInstrument, col, dragPaintState);
                repaint();
            }
        }
        
        void mouseDrag(const juce::MouseEvent& e) override
        {
            if (dragInstrument == Instrument::Count) return;
            
            int col = (e.x - labelW) / cellW;
            
            if (col >= 0 && col < 16 && col != dragLastCol)
            {
                // Paint all columns between last and current
                int start = juce::jmin(dragLastCol, col);
                int end = juce::jmax(dragLastCol, col);
                
                for (int c = start; c <= end; ++c)
                {
                    engine.getSequencer().setStep(dragInstrument, c, dragPaintState);
                }
                
                dragLastCol = col;
                repaint();
            }
        }
        
        void mouseUp(const juce::MouseEvent&) override
        {
            dragInstrument = Instrument::Count;
            dragLastCol = -1;
        }

    private:
        Engine& engine;
        std::function<void(Instrument)> selectInstrumentCb;
        std::function<void(Instrument)> clearAutomationCb;
        int currentStep = 0;
        
        // Drag painting state
        Instrument dragInstrument = Instrument::Count;
        int dragLastCol = -1;
        StepState dragPaintState = StepState::Off;

        juce::Rectangle<int> getNilBoundsForRow(int row) const
        {
            return { 28, row * rowH + 3, labelW - 32, rowH - 6 };
        }
    };

    // =========================================================================
    // Step Button (custom painted)
    // =========================================================================
    class StepButton909 : public juce::Component
    {
    public:
        StepButton909(Engine& e, int idx) : engine(e), index(idx) {}

        void setInstrument(Instrument inst) { currentInstrument = inst; }
        void setIsCurrentStep(bool v) { isCurrentStep = v; }
        int getIndex() const { return index; }

        void refresh()
        {
            auto newState = engine.getSequencer().getStep(currentInstrument, index);
            bool newCurrent = isCurrentStep;
            if (newState != cachedState || newCurrent != cachedCurrent)
            {
                cachedState = newState;
                cachedCurrent = newCurrent;
                repaint();
            }
        }

        void paint(juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat().reduced(1.0f);

            // Group colours
            juce::Colour top, bot, border, textCol;
            if (index < 4 || (index >= 8 && index < 12))
            {
                top = Clr::stepWhiteTop; bot = Clr::stepWhiteBot;
                border = juce::Colour(0xff999999); textCol = Clr::textDark;
            }
            else if (index < 8)
            {
                top = Clr::stepOrgTop; bot = Clr::stepOrgBot;
                border = Clr::orangeDark; textCol = Clr::textWhite;
            }
            else
            {
                top = Clr::stepBlkTop; bot = Clr::stepBlkBot;
                border = juce::Colours::black; textCol = Clr::textWhite;
            }

            // Shadow
            g.setColour(juce::Colours::black.withAlpha(0.2f));
            g.fillRoundedRectangle(b.translated(0, 1.5f), 3.0f);

            // Button body gradient
            juce::ColourGradient grad(top, 0, b.getY(), bot, 0, b.getBottom(), false);
            g.setGradientFill(grad);
            g.fillRoundedRectangle(b, 3.0f);

            // Border
            g.setColour(border);
            g.drawRoundedRectangle(b, 3.0f, 1.0f);

            // LED indicator (top)
            auto ledBounds = juce::Rectangle<float>(b.getCentreX() - 6.0f, b.getY() + 4.0f, 12.0f, 5.0f);
            bool ledActive = (cachedState != StepState::Off) || (isCurrentStep && engine.isRunning());
            const bool isAccent = (cachedState == StepState::Accent);
            const auto accentLed = juce::Colour(0xffff3300);
            const auto onLed = juce::Colour(0xffff8800);

            if (ledActive)
            {
                // Glow
                g.setColour((isAccent ? accentLed : onLed).withAlpha(0.45f));
                g.fillRoundedRectangle(ledBounds.expanded(2), 3.0f);
                g.setColour(isAccent ? accentLed : onLed);
            }
            else
            {
                g.setColour(Clr::ledOff);
            }
            g.fillRoundedRectangle(ledBounds, 2.5f);

            // Highlight on LED
            if (ledActive)
            {
                g.setColour(juce::Colours::white.withAlpha(0.3f));
                g.fillRoundedRectangle(ledBounds.reduced(1).removeFromTop(2), 1.0f);
            }

            // Step number
            g.setColour(textCol);
            g.setFont(juce::Font(10.0f, juce::Font::bold));
            g.drawText(juce::String(index + 1), b.removeFromBottom(14.0f), juce::Justification::centred, false);
        }

        void mouseDown(const juce::MouseEvent&) override
        {
            engine.getSequencer().cycleStep(currentInstrument, index);
            repaint();
        }

    private:
        Engine& engine;
        int index;
        Instrument currentInstrument = Instrument::Kick;
        bool isCurrentStep = false;
        StepState cachedState = StepState::Off;
        bool cachedCurrent = false;
    };

    // =========================================================================
    // Step Button Row (with paint mode)
    // =========================================================================
    class StepButtonRow : public juce::Component
    {
    public:
        StepButtonRow(Engine& e) : engine(e)
        {
            for (int i = 0; i < 16; ++i)
            {
                auto* btn = new StepButton909(engine, i);
                addAndMakeVisible(btn);
                buttons.add(btn);
            }
        }

        void setInstrument(Instrument inst)
        {
            currentInstrument = inst;
            for (auto* btn : buttons)
                btn->setInstrument(inst);
        }

        void setCurrentStep(int step)
        {
            for (int i = 0; i < 16; ++i)
                buttons[i]->setIsCurrentStep(i == step);
        }

        void refresh()
        {
            for (auto* btn : buttons)
                btn->refresh();
        }

        void resized() override
        {
            auto area = getLocalBounds();
            int btnSize = 40;
            int groupGap = 6;
            int btnGap = 2;

            for (int i = 0; i < 16; ++i)
            {
                buttons[i]->setBounds(area.removeFromLeft(btnSize));
                if ((i + 1) % 4 == 0 && i < 15)
                    area.removeFromLeft(groupGap);
                else if (i < 15)
                    area.removeFromLeft(btnGap);
            }
        }

        void mouseDown(const juce::MouseEvent& e) override
        {
            int col = getButtonIndexAt(e.x);
            if (col < 0) return;

            // Check if click is on a button (not in gap)
            if (!buttons[col]->getBounds().contains(e.getPosition()))
                return;

            dragStartX = e.x;
            dragLastCol = col;

            // Off -> On -> Accent -> Off
            auto currentState = engine.getSequencer().getStep(currentInstrument, col);
            if (currentState == StepState::Off)      dragPaintState = StepState::On;
            else if (currentState == StepState::On)  dragPaintState = StepState::Accent;
            else                                      dragPaintState = StepState::Off;

            // Apply to first cell
            engine.getSequencer().setStep(currentInstrument, col, dragPaintState);
            buttons[col]->refresh();
        }

        void mouseDrag(const juce::MouseEvent& e) override
        {
            // Only activate paint mode if moved beyond threshold
            if (std::abs(e.x - dragStartX) < 3)
                return;

            int col = getButtonIndexAt(e.x);
            if (col < 0 || col == dragLastCol)
                return;

            // Paint all columns between last and current
            int start = juce::jmin(dragLastCol, col);
            int end = juce::jmax(dragLastCol, col);

            for (int c = start; c <= end; ++c)
            {
                engine.getSequencer().setStep(currentInstrument, c, dragPaintState);
                buttons[c]->refresh();
            }

            dragLastCol = col;
        }

        void mouseUp(const juce::MouseEvent&) override
        {
            dragLastCol = -1;
        }

    private:
        Engine& engine;
        Instrument currentInstrument = Instrument::Kick;
        juce::OwnedArray<StepButton909> buttons;
        int dragLastCol = -1;
        int dragStartX = 0;
        StepState dragPaintState = StepState::Off;

        int getButtonIndexAt(int x) const
        {
            // Account for button size, gaps, and group gaps
            int btnSize = 40;
            int btnGap = 2;
            int groupGap = 6;
            int pos = 0;

            for (int i = 0; i < 16; ++i)
            {
                if (x >= pos && x < pos + btnSize)
                    return i;

                pos += btnSize;
                if ((i + 1) % 4 == 0 && i < 15)
                    pos += groupGap;
                else if (i < 15)
                    pos += btnGap;
            }

            return -1; // Click in gap
        }
    };

    // =========================================================================
    // Instrument Select Label
    // =========================================================================
    class InstrumentLabel : public juce::Component
    {
    public:
        InstrumentLabel(Instrument inst, const char* text, std::function<void(Instrument)> cb)
            : instrument(inst), label(text), onClick(std::move(cb)) {}

        void setSelected(bool s) { selected = s; repaint(); }
        Instrument getInstrument() const { return instrument; }

        void paint(juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat();
            g.setColour(selected ? Clr::orange : juce::Colour(0xff444444));
            g.fillRoundedRectangle(b, 2.0f);
            g.setColour(selected ? Clr::textWhite : juce::Colour(0xff999999));
            g.setFont(juce::Font(9.0f, juce::Font::bold));
            g.drawText(label, b, juce::Justification::centred, false);
        }

        void mouseDown(const juce::MouseEvent&) override
        {
            if (onClick) onClick(instrument);
        }

    private:
        Instrument instrument;
        juce::String label;
        bool selected = false;
        std::function<void(Instrument)> onClick;
    };

    // =========================================================================
    // Pattern Manager Overlay
    // =========================================================================
    class PatternManagerOverlay : public juce::Component, private juce::Button::Listener
    {
    public:
        std::function<void(int)> onSelectBank;
        std::function<void(int, int)> onSelectPattern;
        std::function<void(int, int, const juce::String&)> onRenamePattern;
        std::function<void(int, int)> onCopyPattern;
        std::function<void(int, int)> onPastePattern;
        std::function<void(int, int)> onRandomizePattern;
        std::function<void(int)> onSaveBank;
        std::function<void()> onClose;

        PatternManagerOverlay()
        {
            addAndMakeVisible(titleLabel);
            titleLabel.setText("PATTERN MANAGER", juce::dontSendNotification);
            titleLabel.setJustificationType(juce::Justification::centredLeft);

            setupButton(closeButton, "CLOSE");
            setupButton(bankAButton, "BANK A");
            setupButton(bankBButton, "BANK B");
            setupButton(copyButton, "COPY");
            setupButton(pasteButton, "PASTE");
            setupButton(randomButton, "RANDOM");
            setupButton(saveButton, "SAVE BANK");
            setupButton(renameButton, "RENAME");

            addAndMakeVisible(nameEditor);
            nameEditor.setTextToShowWhenEmpty("Pattern name", Clr::textLight);
            nameEditor.setInputRestrictions(20);

            for (int i = 0; i < numPatternsPerBank; ++i)
            {
                auto* btn = new juce::TextButton();
                setupButton(*btn, juce::String(i + 1).paddedLeft('0', 2));
                patternButtons.add(btn);
            }
        }

        void setPatternNames(int bank, const std::array<juce::String, numPatternsPerBank>& names)
        {
            patternNames[juce::jlimit(0, numBanks - 1, bank)] = names;
            refreshPatternButtons();
        }

        void setSelection(int bank, int pattern, bool hasPasteData)
        {
            selectedBank = juce::jlimit(0, numBanks - 1, bank);
            selectedPattern = juce::jlimit(0, numPatternsPerBank - 1, pattern);
            canPaste = hasPasteData;
            refreshPatternButtons();
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black.withAlpha(0.76f));

            auto panel = getLocalBounds().reduced(80, 60).toFloat();
            g.setColour(Clr::lcdBg);
            g.fillRoundedRectangle(panel, 8.0f);
            g.setColour(Clr::lcdBorder);
            g.drawRoundedRectangle(panel, 8.0f, 2.0f);

            auto gridArea = panel.reduced(14, 56);
            g.setColour(Clr::gridLine.withAlpha(0.55f));
            for (int i = 1; i < 8; ++i)
            {
                const float x = gridArea.getX() + (gridArea.getWidth() / 8.0f) * i;
                g.drawVerticalLine((int)x, gridArea.getY(), gridArea.getBottom());
            }
            g.drawHorizontalLine((int)(gridArea.getY() + gridArea.getHeight() * 0.5f), gridArea.getX(), gridArea.getRight());
        }

        void resized() override
        {
            auto panel = getLocalBounds().reduced(80, 60);
            auto header = panel.removeFromTop(34);
            titleLabel.setBounds(header.removeFromLeft(220));
            closeButton.setBounds(header.removeFromRight(90).reduced(2));
            bankAButton.setBounds(header.removeFromRight(86).reduced(2));
            bankBButton.setBounds(header.removeFromRight(86).reduced(2));

            auto controls = panel.removeFromBottom(84);
            auto nameRow = controls.removeFromTop(34);
            nameEditor.setBounds(nameRow.removeFromLeft(230).reduced(2));
            renameButton.setBounds(nameRow.removeFromLeft(96).reduced(2));
            saveButton.setBounds(nameRow.removeFromLeft(120).reduced(2));

            auto actionRow = controls.removeFromTop(34);
            copyButton.setBounds(actionRow.removeFromLeft(96).reduced(2));
            pasteButton.setBounds(actionRow.removeFromLeft(96).reduced(2));
            randomButton.setBounds(actionRow.removeFromLeft(110).reduced(2));

            auto grid = panel.reduced(8);
            const int cols = 8;
            const int rows = 2;
            const int w = grid.getWidth() / cols;
            const int h = grid.getHeight() / rows;
            for (int i = 0; i < patternButtons.size(); ++i)
            {
                int row = i / cols;
                int col = i % cols;
                patternButtons[i]->setBounds(grid.getX() + col * w + 3, grid.getY() + row * h + 3, w - 6, h - 6);
            }
        }

    private:
        juce::Label titleLabel;
        juce::TextButton closeButton, bankAButton, bankBButton;
        juce::TextButton copyButton, pasteButton, randomButton, saveButton, renameButton;
        juce::TextEditor nameEditor;
        juce::OwnedArray<juce::TextButton> patternButtons;
        std::array<std::array<juce::String, numPatternsPerBank>, numBanks> patternNames {};
        int selectedBank = 0;
        int selectedPattern = 0;
        bool canPaste = false;

        void setupButton(juce::TextButton& button, const juce::String& text)
        {
            addAndMakeVisible(button);
            button.setButtonText(text);
            button.addListener(this);
            button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2c2c2c));
            button.setColour(juce::TextButton::textColourOffId, Clr::lcdBright);
            button.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
        }

        void refreshPatternButtons()
        {
            for (int i = 0; i < patternButtons.size(); ++i)
            {
                const juce::String id = juce::String(i + 1).paddedLeft('0', 2);
                juce::String nm = patternNames[(size_t)selectedBank][(size_t)i];
                if (nm.isEmpty())
                    nm = id;
                patternButtons[i]->setButtonText(id + "  " + nm.substring(0, 8));
                const bool selected = (i == selectedPattern);
                patternButtons[i]->setColour(juce::TextButton::buttonColourId, selected ? Clr::orange : juce::Colour(0xff222222));
                patternButtons[i]->setColour(juce::TextButton::textColourOffId, selected ? Clr::textWhite : Clr::lcdBright);
            }

            bankAButton.setColour(juce::TextButton::buttonColourId, selectedBank == 0 ? Clr::orange : juce::Colour(0xff2c2c2c));
            bankBButton.setColour(juce::TextButton::buttonColourId, selectedBank == 1 ? Clr::orange : juce::Colour(0xff2c2c2c));
            pasteButton.setEnabled(canPaste);
            nameEditor.setText(patternNames[(size_t)selectedBank][(size_t)selectedPattern], juce::dontSendNotification);
            repaint();
        }

        void buttonClicked(juce::Button* b) override
        {
            if (b == &closeButton)
            {
                if (onClose) onClose();
                return;
            }

            if (b == &bankAButton || b == &bankBButton)
            {
                selectedBank = (b == &bankAButton) ? 0 : 1;
                if (onSelectBank) onSelectBank(selectedBank);
                refreshPatternButtons();
                return;
            }

            if (b == &copyButton)
            {
                if (onCopyPattern) onCopyPattern(selectedBank, selectedPattern);
                canPaste = true;
                refreshPatternButtons();
                return;
            }

            if (b == &pasteButton)
            {
                if (onPastePattern) onPastePattern(selectedBank, selectedPattern);
                return;
            }

            if (b == &randomButton)
            {
                if (onRandomizePattern) onRandomizePattern(selectedBank, selectedPattern);
                return;
            }

            if (b == &saveButton)
            {
                if (onSaveBank) onSaveBank(selectedBank);
                return;
            }

            if (b == &renameButton)
            {
                if (onRenamePattern) onRenamePattern(selectedBank, selectedPattern, nameEditor.getText().trim());
                return;
            }

            for (int i = 0; i < patternButtons.size(); ++i)
            {
                if (b == patternButtons[i])
                {
                    selectedPattern = i;
                    if (onSelectPattern) onSelectPattern(selectedBank, selectedPattern);
                    refreshPatternButtons();
                    return;
                }
            }
        }
    };

    // =========================================================================
    // Main Component
    // =========================================================================
    class MainComponent : public juce::AudioAppComponent, private juce::Timer
    {
    public:
        MainComponent()
        {
            setLookAndFeel(&lf);
            setWantsKeyboardFocus(true);
            initialisePatternBanks();

            // LCD
            lcd = std::make_unique<LCDDisplay>(engine);
            addAndMakeVisible(*lcd);
            lcd->onRequestPatternManager = [this]() { showPatternManager(true); };
            lcd->onBpmChanged = [this](float bpm)
            {
                juce::ignoreUnused(bpm);
                updateCurrentPatternFromEngine();
            };

            // Transport buttons
            setupButton(startBtn, "START", Clr::orange, Clr::textWhite);
            setupButton(stopBtn,  "STOP",  juce::Colours::white, Clr::textDark);
            setupButton(recBtn,   "REC",   juce::Colour(0xffb0b0b0), Clr::textDark);
            setupButton(panelBtn, "PANEL", juce::Colour(0xffbbbbbb), juce::Colour(0xff666666));
            setupButton(listBtn,  "LIST",  juce::Colour(0xffd18652), Clr::textWhite);
            setupButton(helpBtn,  "HELP",  juce::Colours::white, Clr::textDark);

            startBtn.onClick = [this] { engine.setRunning(true); };
            stopBtn.onClick  = [this] { engine.setRunning(false); };
            recBtn.onClick   = [this] { toggleRecording(); };
            panelBtn.onClick = [this] { togglePanel(); };
            listBtn.onClick  = [this] { showPatternManager(true); };

            // Instrument sections
            for (int i = 0; i < numSections; ++i)
            {
                auto* sec = new InstrumentSection(engine, sectionDefs[i],
                    [this](const KnobDef& kd, float value) { onInstrumentKnobMoved(kd, value); });
                addAndMakeVisible(sec);
                sections.add(sec);
            }

            // Sequencer grid (hidden initially)
            auto selectCb = [this](Instrument inst) { selectInstrument(inst); };
            auto clearAutomationCb = [this](Instrument inst) { requestClearTrackAutomation(inst); };
            grid = std::make_unique<SequencerGrid>(engine, selectCb, clearAutomationCb);
            addChildComponent(*grid); // hidden

            // Instrument select labels
            for (int i = 0; i < numGridRows; ++i)
            {
                auto* lbl = new InstrumentLabel(gridRows[i].instrument, gridRows[i].label, selectCb);
                addAndMakeVisible(lbl);
                instLabels.add(lbl);
            }
            instLabels[0]->setSelected(true);

            // Step button row (with paint mode)
            stepButtonRow = std::make_unique<StepButtonRow>(engine);
            stepButtonRow->setInstrument(Instrument::Kick);
            addAndMakeVisible(*stepButtonRow);

            // Pattern manager overlay (right-click on LCD to open)
            patternManager = std::make_unique<PatternManagerOverlay>();
            patternManager->onClose = [this]() { showPatternManager(false); };
            patternManager->onSelectBank = [this](int bank)
            {
                saveCurrentPatternSlot();
                currentBank = bank;
                selectPattern(currentBank, currentPattern, true);
            };
            patternManager->onSelectPattern = [this](int bank, int pattern)
            {
                saveCurrentPatternSlot();
                selectPattern(bank, pattern, true);
            };
            patternManager->onRenamePattern = [this](int bank, int pattern, const juce::String& name)
            {
                patterns[(size_t)bank][(size_t)pattern].name = name.isNotEmpty() ? name : defaultPatternName(bank, pattern);
                updatePatternManagerNames();
                updateLcdPatternText();
            };
            patternManager->onCopyPattern = [this](int bank, int pattern)
            {
                clipboardPattern = patterns[(size_t)bank][(size_t)pattern];
            };
            patternManager->onPastePattern = [this](int bank, int pattern)
            {
                if (!clipboardPattern.has_value())
                    return;
                patterns[(size_t)bank][(size_t)pattern] = *clipboardPattern;
                selectPattern(bank, pattern, true);
                updatePatternManagerNames();
            };
            patternManager->onRandomizePattern = [this](int bank, int pattern)
            {
                patterns[(size_t)bank][(size_t)pattern] = makeRandomPattern((int)juce::Time::currentTimeMillis());
                patterns[(size_t)bank][(size_t)pattern].name = "RAND " + juce::String(pattern + 1).paddedLeft('0', 2);
                selectPattern(bank, pattern, true);
                updatePatternManagerNames();
            };
            patternManager->onSaveBank = [this](int)
            {
                savePatternBanksToDisk();
            };
            addChildComponent(*patternManager);

            selectPattern(currentBank, currentPattern, true);

            setSize(windowW, collapsedHeight);
            startTimerHz(30);
            setAudioChannels(0, 2);
        }

        ~MainComponent() override
        {
            saveCurrentPatternSlot();
            savePatternBanksToDisk();
            setLookAndFeel(nullptr);
            shutdownAudio();
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
        {
            engine.prepare(sampleRate, samplesPerBlockExpected, 2);
            engine.getSequencer().setLength(16);
            applyPattern(patterns[(size_t)currentBank][(size_t)currentPattern]);
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            engine.render(*bufferToFill.buffer, bufferToFill.numSamples);
        }

        void releaseResources() override {}

        int getExpandedHeightForWindow() const { return expandedHeight(); }

        void paint(juce::Graphics& g) override
        {
            auto area = getLocalBounds();

            // Header background (beige)
            g.setColour(Clr::beige);
            g.fillRect(area.removeFromTop(headerH));

            // Header bottom border
            g.setColour(Clr::headerBorder);
            g.drawLine(0, (float)headerH, (float)getWidth(), (float)headerH, 1.0f);

            // Grid area if expanded
            if (panelExpanded)
            {
                auto gridArea = area.removeFromTop(grid->getDesiredHeight());
                g.setColour(Clr::mainGrey);
                g.fillRect(gridArea);
                g.setColour(Clr::headerBorder);
                g.drawLine(0, (float)gridArea.getBottom(), (float)getWidth(), (float)gridArea.getBottom(), 1.0f);
            }

            // Main area (grey)
            auto footerY = getHeight() - footerH;
            g.setColour(Clr::mainGrey);
            g.fillRect(0, area.getY(), getWidth(), footerY - area.getY());

            // Footer
            g.setColour(Clr::footerGrey);
            g.fillRect(0, footerY, getWidth(), footerH);
            g.setColour(juce::Colour(0xff888888));
            g.drawLine(0, (float)footerY, (float)getWidth(), (float)footerY, 1.0f);

            // Logo text
            g.setFont(juce::Font(24.0f, juce::Font::bold));
            g.setColour(Clr::textDark);
            g.drawText("LoS.", 14, 8, 58, 30, juce::Justification::centredLeft, false);
            g.setColour(Clr::orange);
            g.drawText("9x9", 66, 8, 48, 30, juce::Justification::centredLeft, false);
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(10.0f, juce::Font::bold));
            g.drawText("RHYTHM COMPOSER", 116, 13, 100, 18, juce::Justification::centredLeft, false);
            g.setColour(Clr::textLight);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("COMPUTER CONTROLLED", 14, 34, 170, 12, juce::Justification::centredLeft, false);

            // Status bar
            auto statusY = footerY - 36;
            auto statusRect = juce::Rectangle<float>(16.0f, (float)statusY, 420.0f, 28.0f);
            g.setColour(Clr::idleGrey);
            g.fillRoundedRectangle(statusRect, 4.0f);
            g.setColour(juce::Colour(0xff444444));
            g.drawRoundedRectangle(statusRect, 4.0f, 1.0f);

            // RPM badge
            g.setColour(juce::Colour(0xff444444));
            g.fillRoundedRectangle(statusRect.getX() + 8, statusRect.getCentreY() - 7, 28, 14, 2.0f);
            g.setColour(Clr::textWhite);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("RPM", (int)(statusRect.getX() + 8), (int)(statusRect.getCentreY() - 7), 28, 14,
                       juce::Justification::centred, false);

            // Status text
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 9.0f, juce::Font::bold));
            g.setColour(juce::Colour(0xff888888));
            auto statusText = engine.isRunning()
                ? juce::String("SYSTEM ACTIVE - INTERNAL CLOCK SYNCED")
                : juce::String("SYSTEM IDLE - WAITING FOR MIDI CLOCK...");
            g.drawText(statusText, (int)(statusRect.getX() + 44), (int)statusRect.getY(),
                       (int)(statusRect.getWidth() - 66), (int)statusRect.getHeight(),
                       juce::Justification::centredLeft, false);

            // Green dot
            g.setColour(juce::Colour(0xff22c55e));
            g.fillEllipse(statusRect.getRight() - 16, statusRect.getCentreY() - 4, 8, 8);
            g.setColour(juce::Colour(0xff22c55e).withAlpha(0.4f));
            g.fillEllipse(statusRect.getRight() - 18, statusRect.getCentreY() - 6, 12, 12);

            // Footer content
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            auto viewText = panelExpanded
                ? juce::String("LoS.9x9 RHYTHM COMPOSER \xe2\x80\xa2 EXPANDED VIEW")
                : juce::String("LoS.9x9 RHYTHM COMPOSER \xe2\x80\xa2 COLLAPSED VIEW");
            g.drawText(viewText, 0, footerY, getWidth() - 16, footerH, juce::Justification::centredRight, false);

            // Bank buttons in footer
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("BANK", 16, footerY + 4, 30, footerH - 8, juce::Justification::centredLeft, false);

            // Bank A/B selectors
            g.setColour(currentBank == 0 ? Clr::orange : juce::Colours::white);
            g.fillRoundedRectangle(50.0f, (float)footerY + 10.0f, 20.0f, 20.0f, 2.0f);
            g.setColour(currentBank == 0 ? Clr::textWhite : Clr::textMid);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("A", 50, footerY + 10, 20, 20, juce::Justification::centred, false);

            g.setColour(currentBank == 1 ? Clr::orange : juce::Colours::white);
            g.fillRoundedRectangle(74.0f, (float)footerY + 10.0f, 20.0f, 20.0f, 2.0f);
            g.setColour(currentBank == 1 ? Clr::textWhite : Clr::textMid);
            g.drawText("B", 74, footerY + 10, 20, 20, juce::Justification::centred, false);

            // Mode
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("MODE", 110, footerY + 4, 30, footerH - 8, juce::Justification::centredLeft, false);

            g.setColour(juce::Colour(0xff333333));
            g.fillRoundedRectangle(146.0f, (float)footerY + 10.0f, 130.0f, 20.0f, 2.0f);
            g.setColour(Clr::textWhite);
            g.setFont(juce::Font(7.0f, juce::Font::bold));
            const juce::String patternLabel = patterns[(size_t)currentBank][(size_t)currentPattern].name;
            g.drawText("PATTERN " + patternLabel.substring(0, 12), 146, footerY + 10, 130, 20, juce::Justification::centred, false);
        }

        void resized() override
        {
            auto area = getLocalBounds();

            // --- HEADER ---
            auto header = area.removeFromTop(headerH);
            auto topRow = header.removeFromTop(76);
            topRow.removeFromLeft(220); // keep logo area clear for paint()

            // LCD + transport/pattern controls
            auto topRight = topRow;
            lcd->setBounds(topRight.removeFromLeft(270).reduced(4, 4));

            auto btnCol = topRight.reduced(4, 4);
            auto btnRow1 = btnCol.removeFromTop(22);
            startBtn.setBounds(btnRow1.removeFromLeft(68).reduced(1));
            stopBtn.setBounds(btnRow1.removeFromLeft(68).reduced(1));
            auto btnRow2 = btnCol.removeFromTop(20);
            recBtn.setBounds(btnRow2.removeFromLeft(68).reduced(1));
            listBtn.setBounds(btnRow2.removeFromLeft(68).reduced(1));
            auto btnRow3 = btnCol.removeFromTop(20);
            panelBtn.setBounds(btnRow3.removeFromLeft(68).reduced(1));
            helpBtn.setBounds(btnRow3.removeFromLeft(68).reduced(1));

            // Instrument sections (bottom of header)
            auto secRow = header.reduced(8, 2);
            const int gap = 2;
            const int moduleWidth = (secRow.getWidth() - gap * (numSections - 1)) / numSections;
            for (int i = 0; i < numSections; ++i)
            {
                sections[i]->setBounds(secRow.removeFromLeft(moduleWidth));
                if (i < numSections - 1)
                    secRow.removeFromLeft(gap);
            }

            // --- GRID (expanded) ---
            if (panelExpanded)
            {
                auto gridArea = area.removeFromTop(grid->getDesiredHeight());
                grid->setBounds(gridArea);
            }

            // --- FOOTER ---
            auto footer = area.removeFromBottom(footerH);
            (void)footer; // painted only

            // --- MAIN AREA ---
            area.removeFromBottom(38); // status bar space

            // Instrument select row - align with sections
            auto instSelRow = area.removeFromTop(20);
            instSelRow.removeFromLeft(12);
            int labelW = 30;
            int labelGap = 2;
            for (int i = 0; i < instLabels.size(); ++i)
            {
                instLabels[i]->setBounds(instSelRow.removeFromLeft(labelW).reduced(0, 1));
                if (i < instLabels.size() - 1)
                    instSelRow.removeFromLeft(labelGap);
            }

            area.removeFromTop(10);

            // Step button row - align with sections
            auto stepRow = area.removeFromTop(44);
            const int stepButtonsW = 16 * 40 + 12 * 2 + 3 * 6;
            const int stepX = (getWidth() - stepButtonsW) / 2;
            stepButtonRow->setBounds(stepX, stepRow.getY(), stepButtonsW, 40);

            if (patternManager)
                patternManager->setBounds(getLocalBounds());
        }

        bool keyPressed(const juce::KeyPress& key) override
        {
            if (key.getKeyCode() == juce::KeyPress::spaceKey)
            {
                engine.setRunning(!engine.isRunning());
                return true;
            }

            auto triggerFromKey = [this](Instrument inst)
            {
                engine.triggerInstrument(inst, 1.0f);
                selectInstrument(inst);

                if (recordingEnabled)
                {
                    const int step = getRecordStepIndex();
                    engine.getSequencer().setStep(inst, step, StepState::On);
                    updateCurrentPatternFromEngine();
                }
            };

            const juce_wchar kc = key.getTextCharacter();
            switch (kc)
            {
                case 'a':
                case 'A': triggerFromKey(Instrument::Kick); return true;
                case 's':
                case 'S': triggerFromKey(Instrument::Snare); return true;
                case 'd':
                case 'D': triggerFromKey(Instrument::TomLow); return true;
                case 'f':
                case 'F': triggerFromKey(Instrument::TomMid); return true;
                case 'g':
                case 'G': triggerFromKey(Instrument::TomHigh); return true;
                case 'h':
                case 'H': triggerFromKey(Instrument::Rim); return true;
                case 'j':
                case 'J': triggerFromKey(Instrument::Clap); return true;
                case 'k':
                case 'K': triggerFromKey(Instrument::ClosedHat); return true;
                case 'l':
                case 'L': triggerFromKey(Instrument::OpenHat); return true;
                case ';': triggerFromKey(Instrument::Crash); return true;
                case '\'': triggerFromKey(Instrument::Ride); return true;
                default: break;
            }
            return false;
        }

        void mouseDown(const juce::MouseEvent& e) override
        {
            if (patternManager && patternManager->isVisible())
                return;

            const int footerY = getHeight() - footerH;
            if (e.y < footerY)
                return;

            const juce::Rectangle<int> bankA(50, footerY + 10, 20, 20);
            const juce::Rectangle<int> bankB(74, footerY + 10, 20, 20);
            if (bankA.contains(e.getPosition()))
            {
                saveCurrentPatternSlot();
                selectPattern(0, currentPattern, true);
                return;
            }
            if (bankB.contains(e.getPosition()))
            {
                saveCurrentPatternSlot();
                selectPattern(1, currentPattern, true);
            }
        }

    private:
        Engine engine;
        LookAndFeel909 lf;
        Instrument selectedInstrument = Instrument::Kick;
        bool panelExpanded = false;
        bool recordingEnabled = false;

        static constexpr int windowW = 850;
        static constexpr int headerH = 202;
        static constexpr int footerH = 40;
        static constexpr int collapsedHeight = 390;

        int expandedHeight() const { return collapsedHeight + grid->getDesiredHeight(); }

        // Components
        std::unique_ptr<LCDDisplay> lcd;
        juce::TextButton startBtn, stopBtn, recBtn, panelBtn, listBtn, helpBtn;
        juce::OwnedArray<InstrumentSection> sections;
        std::unique_ptr<SequencerGrid> grid;
        std::unique_ptr<StepButtonRow> stepButtonRow;
        juce::OwnedArray<InstrumentLabel> instLabels;
        std::unique_ptr<PatternManagerOverlay> patternManager;
        std::array<std::array<PatternData, numPatternsPerBank>, numBanks> patterns {};
        std::optional<PatternData> clipboardPattern;
        int currentBank = 0;
        int currentPattern = 0;
        bool isApplyingPattern = false;

        juce::String defaultPatternName(int bank, int pattern) const
        {
            return juce::String(pattern + 1).paddedLeft('0', 2)
                + "-"
                + juce::String::charToString((juce_wchar)('A' + bank));
        }

        juce::File patternStorageFile() const
        {
            auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("LoS9x9");
            dir.createDirectory();
            return dir.getChildFile("pattern_banks.json");
        }

        PatternData makeRandomPattern(int seed) const
        {
            juce::Random rng(seed);
            PatternData p;
            p.bpm = 118.0f + rng.nextFloat() * 18.0f;
            p.shuffle = rng.nextFloat() * 0.38f;
            p.accent = 0.4f + rng.nextFloat() * 0.35f;

            auto set = [&p](Instrument inst, int step, StepState st)
            {
                if (step >= 0 && step < 16)
                    p.steps[(int)inst][step] = st;
            };

            // House/techno-informed foundations.
            for (int s : { 0, 4, 8, 12 }) set(Instrument::Kick, s, StepState::On);
            if (rng.nextBool()) set(Instrument::Kick, 10, StepState::On);
            if (rng.nextFloat() > 0.6f) set(Instrument::Kick, 15, StepState::Accent);

            for (int s : { 4, 12 }) set(Instrument::Snare, s, StepState::On);
            if (rng.nextBool()) set(Instrument::Clap, 4, StepState::Accent);
            if (rng.nextBool()) set(Instrument::Clap, 12, StepState::Accent);

            const bool denseHats = rng.nextBool();
            for (int s = 0; s < 16; ++s)
            {
                if (denseHats ? (s % 2 == 0) : (s % 4 == 2))
                    set(Instrument::ClosedHat, s, (rng.nextFloat() > 0.8f) ? StepState::Accent : StepState::On);
            }
            for (int s : { 3, 7, 11, 15 })
                if (rng.nextFloat() > 0.35f) set(Instrument::OpenHat, s, StepState::On);

            if (rng.nextFloat() > 0.65f) set(Instrument::TomLow, 14, StepState::On);
            if (rng.nextFloat() > 0.70f) set(Instrument::TomMid, 15, StepState::On);
            if (rng.nextFloat() > 0.76f) set(Instrument::TomHigh, 13, StepState::On);
            if (rng.nextFloat() > 0.72f) set(Instrument::Rim, 6, StepState::On);
            if (rng.nextFloat() > 0.68f) set(Instrument::Crash, 0, StepState::Accent);
            if (rng.nextFloat() > 0.82f) set(Instrument::Ride, 8, StepState::On);

            p.name = "RND " + juce::String(seed % 100).paddedLeft('0', 2);
            return p;
        }

        void applyPattern(const PatternData& pattern)
        {
            isApplyingPattern = true;

            auto& seq = engine.getSequencer();
            seq.clear();
            seq.clearAllAutomation();
            seq.setLength(16);
            for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            {
                for (int step = 0; step < 16; ++step)
                {
                    seq.setStep((Instrument)inst, step, pattern.steps[inst][step]);
                    for (int param = 0; param < (int)AutomationParam::Count; ++param)
                    {
                        if (pattern.automationActive[inst][param][step])
                        {
                            seq.setAutomationPoint((Instrument)inst, (AutomationParam)param, step,
                                                   pattern.automationValue[inst][param][step]);
                        }
                    }
                }
            }

            seq.setShuffle(pattern.shuffle);
            engine.setAccentLevel(pattern.accent);
            lcd->setShuffleAccent(pattern.shuffle, pattern.accent);
            lcd->setBpmValue(pattern.bpm, true);
            lcd->setPatternDisplay(currentBank, currentPattern, pattern.name);

            isApplyingPattern = false;
        }

        void saveCurrentPatternSlot()
        {
            if (isApplyingPattern)
                return;

            auto& slot = patterns[(size_t)currentBank][(size_t)currentPattern];
            for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            {
                for (int step = 0; step < 16; ++step)
                {
                    slot.steps[inst][step] = engine.getSequencer().getStep((Instrument)inst, step);

                    for (int param = 0; param < (int)AutomationParam::Count; ++param)
                    {
                        float value = 0.0f;
                        const bool active = engine.getSequencer().getAutomationPoint(
                            (Instrument)inst, (AutomationParam)param, step, value);
                        slot.automationActive[inst][param][step] = active;
                        slot.automationValue[inst][param][step] = value;
                    }
                }
            }

            slot.bpm = engine.getSequencer().getBpm();
            slot.shuffle = engine.getSequencer().getShuffle();
            slot.accent = engine.getAccentLevel();
            if (slot.name.isEmpty())
                slot.name = defaultPatternName(currentBank, currentPattern);
        }

        void updateCurrentPatternFromEngine()
        {
            saveCurrentPatternSlot();
            updateLcdPatternText();
        }

        void updateLcdPatternText()
        {
            lcd->setPatternDisplay(currentBank, currentPattern, patterns[(size_t)currentBank][(size_t)currentPattern].name);
        }

        void updatePatternManagerNames()
        {
            if (!patternManager)
                return;

            for (int b = 0; b < numBanks; ++b)
            {
                std::array<juce::String, numPatternsPerBank> names;
                for (int p = 0; p < numPatternsPerBank; ++p)
                    names[(size_t)p] = patterns[(size_t)b][(size_t)p].name;
                patternManager->setPatternNames(b, names);
            }
        }

        void selectPattern(int bank, int pattern, bool refreshUI)
        {
            currentBank = juce::jlimit(0, numBanks - 1, bank);
            currentPattern = juce::jlimit(0, numPatternsPerBank - 1, pattern);
            applyPattern(patterns[(size_t)currentBank][(size_t)currentPattern]);

            if (refreshUI)
            {
                updateLcdPatternText();
                updatePatternManagerNames();
                if (patternManager)
                    patternManager->setSelection(currentBank, currentPattern, clipboardPattern.has_value());
                repaint();
            }
        }

        void showPatternManager(bool show)
        {
            if (!patternManager)
                return;

            if (show)
            {
                saveCurrentPatternSlot();
                updatePatternManagerNames();
                patternManager->setSelection(currentBank, currentPattern, clipboardPattern.has_value());
                patternManager->setVisible(true);
                patternManager->toFront(true);
            }
            else
            {
                patternManager->setVisible(false);
            }
        }

        bool loadPatternBanksFromDisk()
        {
            const auto file = patternStorageFile();
            if (!file.existsAsFile())
                return false;

            const auto json = juce::JSON::parse(file);
            if (!json.isObject())
                return false;

            auto* root = json.getDynamicObject();
            auto banksVar = root->getProperty("banks");
            if (!banksVar.isArray())
                return false;

            auto* banksArr = banksVar.getArray();
            for (int b = 0; b < juce::jmin(numBanks, banksArr->size()); ++b)
            {
                auto bankVar = banksArr->getReference(b);
                if (!bankVar.isArray())
                    continue;
                auto* pattArr = bankVar.getArray();
                for (int p = 0; p < juce::jmin(numPatternsPerBank, pattArr->size()); ++p)
                {
                    auto pattVar = pattArr->getReference(p);
                    if (!pattVar.isObject())
                        continue;
                    auto* obj = pattVar.getDynamicObject();
                    auto& dst = patterns[(size_t)b][(size_t)p];
                    dst.name = obj->getProperty("name").toString();
                    dst.bpm = (float)obj->getProperty("bpm");
                    dst.shuffle = (float)obj->getProperty("shuffle");
                    dst.accent = (float)obj->getProperty("accent");

                    auto stepsStr = obj->getProperty("steps").toString();
                    int idx = 0;
                    for (int inst = 0; inst < (int)Instrument::Count; ++inst)
                    {
                        for (int step = 0; step < 16; ++step)
                        {
                            StepState state = StepState::Off;
                            if (idx < stepsStr.length())
                            {
                                auto c = stepsStr[(int)idx++];
                                if (c == '1') state = StepState::On;
                                else if (c == '2') state = StepState::Accent;
                            }
                            dst.steps[inst][step] = state;
                        }
                    }

                    for (int inst = 0; inst < (int)Instrument::Count; ++inst)
                        for (int param = 0; param < (int)AutomationParam::Count; ++param)
                            for (int step = 0; step < 16; ++step)
                            {
                                dst.automationActive[inst][param][step] = false;
                                dst.automationValue[inst][param][step] = 0.0f;
                            }

                    auto autoVar = obj->getProperty("automation");
                    if (autoVar.isArray())
                    {
                        auto* autoArr = autoVar.getArray();
                        for (int i = 0; i < autoArr->size(); ++i)
                        {
                            auto pointVar = autoArr->getReference(i);
                            if (!pointVar.isObject())
                                continue;

                            auto* point = pointVar.getDynamicObject();
                            const int inst = (int)point->getProperty("i");
                            const int param = (int)point->getProperty("p");
                            const int step = (int)point->getProperty("s");
                            const float value = (float)point->getProperty("v");

                            if (inst < 0 || inst >= (int)Instrument::Count
                                || param < 0 || param >= (int)AutomationParam::Count
                                || step < 0 || step >= 16)
                                continue;

                            dst.automationActive[inst][param][step] = true;
                            dst.automationValue[inst][param][step] = juce::jlimit(0.0f, 1.0f, value);
                        }
                    }
                }
            }

            return true;
        }

        void savePatternBanksToDisk()
        {
            juce::DynamicObject::Ptr root(new juce::DynamicObject());
            juce::Array<juce::var> banksVar;

            for (int b = 0; b < numBanks; ++b)
            {
                juce::Array<juce::var> pattVar;
                for (int p = 0; p < numPatternsPerBank; ++p)
                {
                    const auto& src = patterns[(size_t)b][(size_t)p];
                    juce::String steps;
                    steps.preallocateBytes((int)Instrument::Count * 16);
                    for (int inst = 0; inst < (int)Instrument::Count; ++inst)
                        for (int step = 0; step < 16; ++step)
                            steps += juce::String((int)src.steps[inst][step]);

                    juce::DynamicObject::Ptr patt(new juce::DynamicObject());
                    patt->setProperty("name", src.name);
                    patt->setProperty("bpm", src.bpm);
                    patt->setProperty("shuffle", src.shuffle);
                    patt->setProperty("accent", src.accent);
                    patt->setProperty("steps", steps);

                    juce::Array<juce::var> automation;
                    for (int inst = 0; inst < (int)Instrument::Count; ++inst)
                    {
                        for (int param = 0; param < (int)AutomationParam::Count; ++param)
                        {
                            for (int step = 0; step < 16; ++step)
                            {
                                if (!src.automationActive[inst][param][step])
                                    continue;

                                juce::DynamicObject::Ptr point(new juce::DynamicObject());
                                point->setProperty("i", inst);
                                point->setProperty("p", param);
                                point->setProperty("s", step);
                                point->setProperty("v", src.automationValue[inst][param][step]);
                                automation.add(juce::var(point.get()));
                            }
                        }
                    }
                    patt->setProperty("automation", juce::var(automation));
                    pattVar.add(juce::var(patt.get()));
                }
                banksVar.add(juce::var(pattVar));
            }

            root->setProperty("banks", juce::var(banksVar));
            patternStorageFile().replaceWithText(juce::JSON::toString(juce::var(root.get())));
        }

        void initialisePatternBanks()
        {
            for (int b = 0; b < numBanks; ++b)
            {
                for (int p = 0; p < numPatternsPerBank; ++p)
                {
                    patterns[(size_t)b][(size_t)p].name = defaultPatternName(b, p);
                    patterns[(size_t)b][(size_t)p].bpm = 120.0f;
                    patterns[(size_t)b][(size_t)p].shuffle = 0.0f;
                    patterns[(size_t)b][(size_t)p].accent = 0.5f;
                }
            }

            // Demo set lives in bank B.
            for (int p = 0; p < numPatternsPerBank; ++p)
            {
                patterns[1][(size_t)p] = makeRandomPattern(100 + p * 17);
                patterns[1][(size_t)p].name = "DEMO " + juce::String(p + 1).paddedLeft('0', 2);
            }

            // Signature 4-on-the-floor default for A01.
            auto& a01 = patterns[0][0];
            for (int s : { 0, 4, 8, 12 }) a01.steps[(int)Instrument::Kick][s] = StepState::On;
            for (int s : { 4, 12 }) a01.steps[(int)Instrument::Snare][s] = StepState::On;
            for (int s = 0; s < 16; s += 2) a01.steps[(int)Instrument::ClosedHat][s] = StepState::On;
            a01.name = "A01 BASIC";

            loadPatternBanksFromDisk();
        }

        void setupButton(juce::TextButton& btn, const juce::String& text,
                         juce::Colour bg, juce::Colour textCol)
        {
            btn.setButtonText(text);
            btn.setColour(juce::TextButton::buttonColourId, bg);
            btn.setColour(juce::TextButton::textColourOffId, textCol);
            addAndMakeVisible(btn);
        }

        juce::String instrumentName(Instrument inst) const
        {
            switch (inst)
            {
                case Instrument::Kick:      return "Bass Drum";
                case Instrument::Snare:     return "Snare Drum";
                case Instrument::TomLow:    return "Low Tom";
                case Instrument::TomMid:    return "Mid Tom";
                case Instrument::TomHigh:   return "Hi Tom";
                case Instrument::Rim:       return "Rim";
                case Instrument::Clap:      return "Clap";
                case Instrument::ClosedHat: return "Closed Hat";
                case Instrument::OpenHat:   return "Open Hat";
                case Instrument::Crash:     return "Crash";
                case Instrument::Ride:      return "Ride";
                default: break;
            }
            return "Track";
        }

        int getRecordStepIndex()
        {
            return juce::jlimit(0, 15, engine.getSequencer().getCurrentStep());
        }

        void toggleRecording()
        {
            recordingEnabled = !recordingEnabled;
            recBtn.setColour(juce::TextButton::buttonColourId,
                             recordingEnabled ? juce::Colour(0xffcc3030) : juce::Colour(0xffb0b0b0));
            recBtn.setColour(juce::TextButton::textColourOffId,
                             recordingEnabled ? juce::Colours::white : Clr::textDark);
            repaint();
        }

        void onInstrumentKnobMoved(const KnobDef& kd, float value)
        {
            if (!recordingEnabled)
                return;

            const int step = getRecordStepIndex();
            engine.getSequencer().setAutomationPoint(kd.instrument, toAutomationParam(kd.param), step, value);
            updateCurrentPatternFromEngine();

            if (panelExpanded)
                grid->repaint();
        }

        void requestClearTrackAutomation(Instrument inst)
        {
            if (!engine.getSequencer().hasAutomation(inst))
                return;

            const auto confirm = juce::AlertWindow::showOkCancelBox(
                juce::AlertWindow::WarningIcon,
                "Remove Recorded Knob Movements",
                "Clear all recorded knob movement data for " + instrumentName(inst) + "?",
                "Clear",
                "Cancel",
                this,
                nullptr);

            if (!confirm)
                return;

            engine.getSequencer().clearAutomation(inst);
            saveCurrentPatternSlot();
            if (panelExpanded)
                grid->repaint();
        }

        void togglePanel()
        {
            panelExpanded = !panelExpanded;
            grid->setVisible(panelExpanded);

            // Update panel button style
            panelBtn.setColour(juce::TextButton::buttonColourId,
                               panelExpanded ? Clr::orange : juce::Colour(0xffbbbbbb));
            panelBtn.setColour(juce::TextButton::textColourOffId,
                               panelExpanded ? Clr::textWhite : juce::Colour(0xff666666));

            setSize(getWidth(), panelExpanded ? expandedHeight() : collapsedHeight);
        }

        void selectInstrument(Instrument inst)
        {
            selectedInstrument = inst;
            for (auto* lbl : instLabels)
                lbl->setSelected(lbl->getInstrument() == inst);
            stepButtonRow->setInstrument(inst);
        }

        void timerCallback() override
        {
            updateCurrentPatternFromEngine();
            int currentStep = engine.getSequencer().getCurrentStep();

            stepButtonRow->setCurrentStep(currentStep);
            stepButtonRow->refresh();

            if (panelExpanded)
            {
                grid->setCurrentStep(currentStep);
                grid->repaint();
            }

            lcd->repaint();
            repaint(); // for status bar text
        }
    };

    // =========================================================================
    // Window & Application
    // =========================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name, Clr::mainGrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            setResizable(false, false);

            const auto displayArea = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
            int expandedH = getHeight();
            if (auto* main = dynamic_cast<MainComponent*>(getContentComponent()))
                expandedH = main->getExpandedHeightForWindow();

            const int x = displayArea.getX() + (displayArea.getWidth() - getWidth()) / 2;
            int y = displayArea.getY() + 18;
            const int maxTopForExpanded = displayArea.getBottom() - expandedH - 10;
            if (maxTopForExpanded < y)
                y = juce::jmax(displayArea.getY(), maxTopForExpanded);

            setTopLeftPosition(x, y);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    class LoS9x9Application : public juce::JUCEApplication
    {
    public:
        const juce::String getApplicationName() override { return "LoS.9x9 Rhythm Composer"; }
        const juce::String getApplicationVersion() override { return "0.3.0"; }
        void initialise(const juce::String&) override { mainWindow.reset(new MainWindow(getApplicationName())); }
        void shutdown() override { mainWindow = nullptr; }

    private:
        std::unique_ptr<MainWindow> mainWindow;
    };
}

START_JUCE_APPLICATION(rb338::LoS9x9Application)
