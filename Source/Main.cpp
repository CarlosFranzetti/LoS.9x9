#include <JuceHeader.h>
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
            { "Decay", Instrument::TomLow, ParamType::Decay },
            { "Level", Instrument::TomLow, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level } // unused
        }, 3, true },
        { "Mid Tom", {
            { "Tune",  Instrument::TomMid, ParamType::Tune },
            { "Decay", Instrument::TomMid, ParamType::Decay },
            { "Level", Instrument::TomMid, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level }
        }, 3, true },
        { "Rim / Clap", {
            { "Rim Lvl",  Instrument::Rim,  ParamType::Level },
            { "Clap Lvl", Instrument::Clap, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level }
        }, 2, true },
        { "Hi Hats", {
            { "CH Level", Instrument::ClosedHat, ParamType::Level },
            { "OH Level", Instrument::OpenHat,   ParamType::Level },
            { "Tune",     Instrument::ClosedHat, ParamType::Tune },
            { "Tone",     Instrument::ClosedHat, ParamType::Tone }
        }, 4, true },
        { "Cymbals", {
            { "Crash Lvl", Instrument::Crash, ParamType::Level },
            { "Ride Lvl",  Instrument::Ride,  ParamType::Level },
            { "", Instrument::Kick, ParamType::Level },
            { "", Instrument::Kick, ParamType::Level }
        }, 2, false }
    };
    static constexpr int numSections = 7;

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

            // Bottom row
            g.setColour(Clr::lcdBright);
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 24.0f, juce::Font::bold));
            g.drawText("01-A", inner, juce::Justification::centredLeft, false);

            float bpm = engine.getSequencer().isRunning() ? currentBpm : currentBpm;
            g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 28.0f, juce::Font::bold));
            g.drawText(juce::String(bpm, 2), inner, juce::Justification::centredRight, false);

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
                currentBpm = juce::jlimit(60.0f, 180.0f, currentBpm + wheel.deltaY * 2.0f);
                engine.setBpm(currentBpm);
                repaint();
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
                    currentBpm = newBpm;
                    engine.setBpm(currentBpm);
                    repaint();
                }
            }
        }

        void mouseDown(const juce::MouseEvent&) override
        {
            dragStartBpm = currentBpm;
        }

        float currentBpm = 120.0f;

    private:
        Engine& engine;
        float dragStartBpm = 120.0f;
        juce::Slider shuffleSlider;
        juce::Slider accentSlider;

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
        InstrumentSection(Engine& e, const SectionDef& def)
            : engine(e), sectionDef(def)
        {
            for (int i = 0; i < def.numKnobs; ++i)
            {
                auto* knob = new juce::Slider();
                knob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
                knob->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
                knob->setRange(0.0, 1.0, 0.01);
                knob->setValue(getInitialValue(def.knobs[i]));
                knob->addListener(this);
                knob->setDoubleClickReturnValue(true, 0.5); // Double-click resets to 0.5 (center)
                addAndMakeVisible(knob);
                knobs.add(knob);
            }
        }

        void paint(juce::Graphics& g) override
        {
            auto b = getLocalBounds();

            // Title
            g.setColour(Clr::textDark);
            g.setFont(juce::Font(10.0f, juce::Font::bold));
            g.drawText(juce::String(sectionDef.title).toUpperCase(), b.removeFromTop(16).reduced(2, 0),
                       juce::Justification::centred, false);

            // Knob labels
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            for (int i = 0; i < sectionDef.numKnobs; ++i)
            {
                if (knobs[i] != nullptr)
                {
                    auto kb = knobs[i]->getBounds();
                    // Reduced extension from ±8px to ±4px to prevent overlap
                    g.drawText(juce::String(sectionDef.knobs[i].label).toUpperCase(),
                               kb.getX() - 4, kb.getBottom() + 3, kb.getWidth() + 8, 12,
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
            b.removeFromTop(20); // title
            b = b.reduced(4, 0); // Reduced from 8 for more horizontal space

            int cols = 2;
            int knobSize = 38;
            int vGap = 20; // Space for label below knob
            int hGap = (b.getWidth() - cols * knobSize) / (cols + 1);
            if (hGap < 4) hGap = 4; // Minimum gap increased from 2 to 4

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
                            // Hi-hats share tune/tone parameters
                            if (kd.instrument == Instrument::ClosedHat)
                            {
                                engine.getChannel(Instrument::OpenHat).params.tune = val;
                                engine.updateInstrumentSound(Instrument::OpenHat);
                            }
                            break;
                        case ParamType::Decay:  ch.params.decay = val;  needResynth = true; break;
                        case ParamType::Tone:
                            ch.params.tone = val;
                            needResynth = true;
                            // Hi-hats share tune/tone parameters
                            if (kd.instrument == Instrument::ClosedHat)
                            {
                                engine.getChannel(Instrument::OpenHat).params.tone = val;
                                engine.updateInstrumentSound(Instrument::OpenHat);
                            }
                            break;
                        case ParamType::Snappy: ch.params.snappy = val; needResynth = true; break;
                    }

                    if (needResynth)
                        engine.updateInstrumentSound(kd.instrument);
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
        SequencerGrid(Engine& e, std::function<void(Instrument)> onSelectInst)
            : engine(e), selectInstrumentCb(std::move(onSelectInst)) {}

        static constexpr int rowH = 30;
        static constexpr int labelW = 44;
        static constexpr int cellW = 48;

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
                g.drawText(gridRows[row].label, 0, y, labelW, rowH, juce::Justification::centred, false);

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

            // Click on label = select instrument
            if (e.x < labelW)
            {
                if (selectInstrumentCb)
                    selectInstrumentCb(gridRows[row].instrument);
                return;
            }

            if (col >= 0 && col < 16)
            {
                dragInstrument = gridRows[row].instrument;
                dragLastCol = col;
                
                // Determine paint state: if step is Off, we paint On; if On or Accent, we paint Off
                auto currentState = engine.getSequencer().getStep(dragInstrument, col);
                dragPaintState = (currentState == StepState::Off) ? StepState::On : StepState::Off;
                
                // Apply to first cell
                if (dragPaintState == StepState::Off)
                {
                    engine.getSequencer().setStep(dragInstrument, col, StepState::Off);
                }
                else
                {
                    engine.getSequencer().setStep(dragInstrument, col, StepState::On);
                }
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
        int currentStep = 0;
        
        // Drag painting state
        Instrument dragInstrument = Instrument::Count;
        int dragLastCol = -1;
        StepState dragPaintState = StepState::Off;
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

            if (ledActive)
            {
                // Glow
                g.setColour(Clr::ledOn.withAlpha(0.4f));
                g.fillRoundedRectangle(ledBounds.expanded(2), 3.0f);
                g.setColour(Clr::ledOn);
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
            int btnSize = 48;
            int groupGap = 10;
            int btnGap = 3;

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

            // Determine paint state: opposite of current cell
            auto currentState = engine.getSequencer().getStep(currentInstrument, col);
            dragPaintState = (currentState == StepState::Off) ? StepState::On : StepState::Off;

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
            int btnSize = 48;
            int btnGap = 3;
            int groupGap = 10;
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
    // Main Component
    // =========================================================================
    class MainComponent : public juce::AudioAppComponent, private juce::Timer
    {
    public:
        MainComponent()
        {
            setLookAndFeel(&lf);
            setWantsKeyboardFocus(true);

            // LCD
            lcd = std::make_unique<LCDDisplay>(engine);
            addAndMakeVisible(*lcd);

            // Transport buttons
            setupButton(startBtn, "START", Clr::orange, Clr::textWhite);
            setupButton(stopBtn,  "STOP",  juce::Colours::white, Clr::textDark);
            setupButton(panelBtn, "PANEL", juce::Colour(0xffbbbbbb), juce::Colour(0xff666666));
            setupButton(helpBtn,  "HELP",  juce::Colours::white, Clr::textDark);

            startBtn.onClick = [this] { engine.setRunning(true); };
            stopBtn.onClick  = [this] { engine.setRunning(false); };
            panelBtn.onClick = [this] { togglePanel(); };

            // Instrument sections
            for (int i = 0; i < numSections; ++i)
            {
                auto* sec = new InstrumentSection(engine, sectionDefs[i]);
                addAndMakeVisible(sec);
                sections.add(sec);
            }

            // Sequencer grid (hidden initially)
            auto selectCb = [this](Instrument inst) { selectInstrument(inst); };
            grid = std::make_unique<SequencerGrid>(engine, selectCb);
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

            setSize(1024, collapsedHeight);
            startTimerHz(30);
            setAudioChannels(0, 2);
        }

        ~MainComponent() override
        {
            setLookAndFeel(nullptr);
            shutdownAudio();
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
        {
            engine.prepare(sampleRate, samplesPerBlockExpected, 2);
            engine.setBpm(lcd->currentBpm);
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            engine.render(*bufferToFill.buffer, bufferToFill.numSamples);
        }

        void releaseResources() override {}

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
            g.setFont(juce::Font(30.0f, juce::Font::bold));
            g.setColour(Clr::textDark);
            g.drawText("LoS.", 16, 10, 70, 36, juce::Justification::centredLeft, false);
            g.setColour(Clr::orange);
            g.drawText("9x9", 72, 10, 60, 36, juce::Justification::centredLeft, false);
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(14.0f, juce::Font::bold));
            g.drawText("RHYTHM COMPOSER", 140, 16, 200, 22, juce::Justification::centredLeft, false);
            g.setColour(Clr::textLight);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("COMPUTER CONTROLLED", 16, 42, 200, 12, juce::Justification::centredLeft, false);

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

            // Bank A (active)
            g.setColour(Clr::orange);
            g.fillRoundedRectangle(50.0f, (float)footerY + 10.0f, 20.0f, 20.0f, 2.0f);
            g.setColour(Clr::textWhite);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("A", 50, footerY + 10, 20, 20, juce::Justification::centred, false);

            // Bank B (inactive)
            g.setColour(juce::Colours::white);
            g.fillRoundedRectangle(74.0f, (float)footerY + 10.0f, 20.0f, 20.0f, 2.0f);
            g.setColour(Clr::textMid);
            g.drawText("B", 74, footerY + 10, 20, 20, juce::Justification::centred, false);

            // Mode
            g.setColour(Clr::textMid);
            g.setFont(juce::Font(8.0f, juce::Font::bold));
            g.drawText("MODE", 110, footerY + 4, 30, footerH - 8, juce::Justification::centredLeft, false);

            g.setColour(juce::Colour(0xff333333));
            g.fillRoundedRectangle(146.0f, (float)footerY + 10.0f, 80.0f, 20.0f, 2.0f);
            g.setColour(Clr::textWhite);
            g.setFont(juce::Font(7.0f, juce::Font::bold));
            g.drawText("PATTERN WRITE", 146, footerY + 10, 80, 20, juce::Justification::centred, false);
        }

        void resized() override
        {
            auto area = getLocalBounds();

            // --- HEADER ---
            auto header = area.removeFromTop(headerH);

            // LCD + buttons (top-right of header)
            auto topRight = header.removeFromRight(440); // Increased for accent knob
            topRight.removeFromTop(8);
            auto lcdRow = topRight.removeFromTop(60);
            lcd->setBounds(lcdRow.removeFromLeft(280).reduced(4)); // Wider for both knobs

            auto btnCol = lcdRow;
            btnCol.removeFromLeft(8);
            auto btnRow1 = btnCol.removeFromTop(32);
            startBtn.setBounds(btnRow1.removeFromLeft(64).reduced(2));
            stopBtn.setBounds(btnRow1.removeFromLeft(64).reduced(2));
            auto btnRow2 = btnCol.removeFromTop(28);
            panelBtn.setBounds(btnRow2.removeFromLeft(60).reduced(2));
            helpBtn.setBounds(btnRow2.removeFromLeft(60).reduced(2));

            // Instrument sections (bottom of header)
            auto secRow = header.removeFromBottom(140);  // Increased for label space + padding from border
            secRow.removeFromBottom(10); // Extra padding above the header border line
            secRow.removeFromLeft(8);
            secRow.removeFromRight(8);
            int secW = secRow.getWidth() / numSections;
            for (int i = 0; i < numSections; ++i)
                sections[i]->setBounds(secRow.removeFromLeft(secW));

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
            area.removeFromBottom(40); // status bar space

            // Instrument select row
            auto instSelRow = area.removeFromTop(22);
            instSelRow.removeFromLeft(16);
            int labelW = 36;
            int labelGap = 3;
            for (int i = 0; i < instLabels.size(); ++i)
            {
                instLabels[i]->setBounds(instSelRow.removeFromLeft(labelW).reduced(0, 1));
                if (i < instLabels.size() - 1)
                    instSelRow.removeFromLeft(labelGap);
            }

            area.removeFromTop(18); // Increased from 8 for better readability

            // Step button row
            auto stepRow = area.removeFromTop(52);
            stepRow.removeFromLeft(16);
            stepButtonRow->setBounds(stepRow);
        }

        bool keyPressed(const juce::KeyPress& key) override
        {
            if (key.getKeyCode() == juce::KeyPress::spaceKey)
            {
                engine.setRunning(!engine.isRunning());
                return true;
            }
            return false;
        }

    private:
        Engine engine;
        LookAndFeel909 lf;
        Instrument selectedInstrument = Instrument::Kick;
        bool panelExpanded = false;

        static constexpr int headerH = 220;  // Increased from 190 for knob label visibility
        static constexpr int footerH = 40;
        static constexpr int collapsedHeight = 430; // Adjusted for new header height

        int expandedHeight() const { return collapsedHeight + grid->getDesiredHeight(); }

        // Components
        std::unique_ptr<LCDDisplay> lcd;
        juce::TextButton startBtn, stopBtn, panelBtn, helpBtn;
        juce::OwnedArray<InstrumentSection> sections;
        std::unique_ptr<SequencerGrid> grid;
        std::unique_ptr<StepButtonRow> stepButtonRow;
        juce::OwnedArray<InstrumentLabel> instLabels;

        void setupButton(juce::TextButton& btn, const juce::String& text,
                         juce::Colour bg, juce::Colour textCol)
        {
            btn.setButtonText(text);
            btn.setColour(juce::TextButton::buttonColourId, bg);
            btn.setColour(juce::TextButton::textColourOffId, textCol);
            addAndMakeVisible(btn);
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
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    class RB338RenewedApplication : public juce::JUCEApplication
    {
    public:
        const juce::String getApplicationName() override { return "LoS.9x9 Rhythm Composer"; }
        const juce::String getApplicationVersion() override { return "0.2.0"; }
        void initialise(const juce::String&) override { mainWindow.reset(new MainWindow(getApplicationName())); }
        void shutdown() override { mainWindow = nullptr; }

    private:
        std::unique_ptr<MainWindow> mainWindow;
    };
}

START_JUCE_APPLICATION(rb338::RB338RenewedApplication)
