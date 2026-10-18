#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "ciphers/SimpleSubstitution.h"
#include "ciphers/DoubleTransposition.h"
#include "ciphers/OneTimePad.h"
#include "utils/Comparison.h"

class MainComponent final : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    struct CipherPanel
    {
        juce::GroupComponent group;
        juce::Label keyLabel;
        juce::TextEditor keyEditor;
        juce::Label outputLabel;
        juce::TextEditor outputEditor;
        juce::Label statsLabel;
        juce::TextButton decryptButton { "Decrypt output" };
        juce::TextEditor decryptedEditor;
        juce::Label decryptedLabel;
    };

    void configurePanel(CipherPanel& panel,
                        const juce::String& title,
                        const juce::String& keyLabel,
                        const juce::String& initialKey);
    void encryptAll();
    void decryptPanel(int panelIndex);
    void updateStatus(const juce::String& message, bool isError = false);
    static juce::String toJuice(const std::string& value);
    static std::string toStd(const juce::String& value);

    juce::Label titleLabel;
    juce::Label subtitleLabel;
    juce::Label plaintextLabel;
    juce::TextEditor plaintextEditor;
    juce::TextButton encryptButton { "Encrypt with all three" };
    juce::TextButton clearButton { "Clear" };
    juce::Label normalizationLabel;
    juce::Label statusLabel;

    CipherPanel substitutionPanel;
    CipherPanel transpositionPanel;
    CipherPanel otpPanel;

    SimpleSubstitution substitution;
    DoubleTransposition transposition;
    OneTimePad otp;

    ComparisonResult substitutionResult;
    ComparisonResult transpositionResult;
    ComparisonResult otpResult;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
