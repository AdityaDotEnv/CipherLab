#include "MainComponent.h"

#include <chrono>
#include <sstream>

namespace
{
    const juce::Colour backgroundColour { 20, 24, 32 };
    const juce::Colour panelColour { 30, 36, 47 };
    const juce::Colour accentColour { 95, 170, 255 };
    const juce::Colour mutedColour { 169, 181, 199 };

    void styleEditor(juce::TextEditor& editor, bool multiline = false)
    {
        editor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(16, 20, 27));
        editor.setColour(juce::TextEditor::textColourId, juce::Colour(232, 238, 247));
        editor.setColour(juce::TextEditor::outlineColourId, juce::Colour(63, 75, 94));
        editor.setColour(juce::TextEditor::focusedOutlineColourId, accentColour);
        editor.setFont(juce::Font(14.0f));
        editor.setMultiLine(multiline, true);
        editor.setReturnKeyStartsNewLine(multiline);
        editor.setScrollbarsShown(true);
    }

    juce::String statText(const ComparisonResult& result)
    {
        return "Ciphertext: " + juce::String(static_cast<juce::int64>(result.ciphertext.size()))
            + " chars   |   Encrypt: " + juce::String(result.encryptionMicroseconds) + " Âµs"
            + "   |   Round-trip: " + (result.roundTripPassed ? "PASS" : "FAIL");
    }
}

MainComponent::MainComponent()
{
    setSize(1180, 820);

    titleLabel.setText("CipherLab", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(30.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(242, 246, 252));
    addAndMakeVisible(titleLabel);

    subtitleLabel.setText("Information & Network Security  â€¢  Classical and modern cipher comparison",
                         juce::dontSendNotification);
    subtitleLabel.setFont(juce::Font(14.0f));
    subtitleLabel.setColour(juce::Label::textColourId, mutedColour);
    addAndMakeVisible(subtitleLabel);

    plaintextLabel.setText("Plaintext", juce::dontSendNotification);
    plaintextLabel.setColour(juce::Label::textColourId, juce::Colour(225, 233, 245));
    plaintextLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    addAndMakeVisible(plaintextLabel);

    styleEditor(plaintextEditor, true);
    plaintextEditor.setText("ATTACK AT DAWN", juce::dontSendNotification);
    plaintextEditor.setTextToShowWhenEmpty("Enter a message to encrypt...", mutedColour);
    addAndMakeVisible(plaintextEditor);

    encryptButton.setColour(juce::TextButton::buttonColourId, accentColour);
    encryptButton.setColour(juce::TextButton::textColourOffId, juce::Colour(12, 19, 29));
    encryptButton.onClick = [this] { encryptAll(); };
    addAndMakeVisible(encryptButton);

    clearButton.setColour(juce::TextButton::buttonColourId, juce::Colour(54, 64, 80));
    clearButton.onClick = [this]
    {
        plaintextEditor.clear();
        substitutionPanel.outputEditor.clear();
        transpositionPanel.outputEditor.clear();
        otpPanel.outputEditor.clear();
        substitutionPanel.decryptedEditor.clear();
        transpositionPanel.decryptedEditor.clear();
        otpPanel.decryptedEditor.clear();
        substitutionPanel.statsLabel.setText({}, juce::dontSendNotification);
        transpositionPanel.statsLabel.setText({}, juce::dontSendNotification);
        otpPanel.statsLabel.setText({}, juce::dontSendNotification);
        statusLabel.setText("Cleared. Enter plaintext and encrypt again.", juce::dontSendNotification);
        statusLabel.setColour(juce::Label::textColourId, mutedColour);
    };
    addAndMakeVisible(clearButton);

    normalizationLabel.setText("For a fair comparison, input is normalized to Aâ€“Z; spaces, digits and punctuation are removed.",
                               juce::dontSendNotification);
    normalizationLabel.setColour(juce::Label::textColourId, mutedColour);
    normalizationLabel.setFont(juce::Font(12.0f));
    addAndMakeVisible(normalizationLabel);

    statusLabel.setFont(juce::Font(13.0f, juce::Font::bold));
    statusLabel.setColour(juce::Label::textColourId, mutedColour);
    addAndMakeVisible(statusLabel);

    configurePanel(substitutionPanel, "1. Simple Substitution", "Alphabet permutation", "QWERTYUIOPASDFGHJKLZXCVBNM");
    configurePanel(transpositionPanel, "2. Double Transposition", "Column keys (Aâ€“Z)", "ZEBRA,CIPHER");
    configurePanel(otpPanel, "3. One-Time Pad", "Key (generated per message)", "");

    substitutionPanel.keyEditor.setReadOnly(false);
    substitutionPanel.keyEditor.setTooltip("Exactly 26 unique letters: the cipher alphabet for A through Z.");
    transpositionPanel.keyEditor.setTooltip("Two comma-separated keywords, for example ZEBRA,CIPHER.");
    otpPanel.keyEditor.setReadOnly(true);
    otpPanel.keyEditor.setText("Generated automatically", juce::dontSendNotification);

    substitutionPanel.decryptButton.onClick = [this] { decryptPanel(0); };
    transpositionPanel.decryptButton.onClick = [this] { decryptPanel(1); };
    otpPanel.decryptButton.onClick = [this] { decryptPanel(2); };

    encryptAll();
}

void MainComponent::configurePanel(CipherPanel& panel,
                                   const juce::String& title,
                                   const juce::String& keyLabelText,
                                   const juce::String& initialKey)
{
    panel.group.setText(title);
    panel.group.setColour(juce::GroupComponent::outlineColourId, juce::Colour(64, 77, 98));
    panel.group.setColour(juce::GroupComponent::textColourId, juce::Colour(222, 233, 248));
    addAndMakeVisible(panel.group);

    panel.keyLabel.setText(keyLabelText, juce::dontSendNotification);
    panel.keyLabel.setColour(juce::Label::textColourId, mutedColour);
    panel.keyLabel.setFont(juce::Font(12.0f));
    addAndMakeVisible(panel.keyLabel);

    styleEditor(panel.keyEditor);
    panel.keyEditor.setText(initialKey, juce::dontSendNotification);
    addAndMakeVisible(panel.keyEditor);

    panel.outputLabel.setText("Ciphertext", juce::dontSendNotification);
    panel.outputLabel.setColour(juce::Label::textColourId, juce::Colour(225, 233, 245));
    panel.outputLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    addAndMakeVisible(panel.outputLabel);

    styleEditor(panel.outputEditor, true);
    panel.outputEditor.setReadOnly(true);
    addAndMakeVisible(panel.outputEditor);

    panel.statsLabel.setColour(juce::Label::textColourId, mutedColour);
    panel.statsLabel.setFont(juce::Font(11.5f));
    addAndMakeVisible(panel.statsLabel);

    panel.decryptButton.setColour(juce::TextButton::buttonColourId, panelColour);
    panel.decryptButton.setColour(juce::TextButton::textColourOffId, juce::Colour(220, 230, 244));
    addAndMakeVisible(panel.decryptButton);

    panel.decryptedLabel.setText("Decrypted plaintext", juce::dontSendNotification);
    panel.decryptedLabel.setColour(juce::Label::textColourId, mutedColour);
    panel.decryptedLabel.setFont(juce::Font(12.0f));
    addAndMakeVisible(panel.decryptedLabel);

    styleEditor(panel.decryptedEditor, true);
    panel.decryptedEditor.setReadOnly(true);
    addAndMakeVisible(panel.decryptedEditor);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColour);
    g.setColour(juce::Colour(36, 44, 58));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10.0f), 12.0f);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(22);
    titleLabel.setBounds(area.removeFromTop(38));
    subtitleLabel.setBounds(area.removeFromTop(26));
    area.removeFromTop(8);

    plaintextLabel.setBounds(area.removeFromTop(22));
    plaintextEditor.setBounds(area.removeFromTop(78));
    area.removeFromTop(8);

    auto actionRow = area.removeFromTop(34);
    encryptButton.setBounds(actionRow.removeFromLeft(210));
    actionRow.removeFromLeft(10);
    clearButton.setBounds(actionRow.removeFromLeft(90));
    area.removeFromTop(4);
    normalizationLabel.setBounds(area.removeFromTop(20));
    statusLabel.setBounds(area.removeFromTop(24));
    area.removeFromTop(4);

    const int gap = 12;
    const int panelWidth = (area.getWidth() - gap * 2) / 3;
    auto left = area.removeFromLeft(panelWidth);
    area.removeFromLeft(gap);
    auto middle = area.removeFromLeft(panelWidth);
    area.removeFromLeft(gap);
    auto right = area;

    auto layoutPanel = [this](CipherPanel& p, juce::Rectangle<int> r)
    {
        p.group.setBounds(r);
        auto inner = r.reduced(12, 28);
        p.keyLabel.setBounds(inner.removeFromTop(18));
        p.keyEditor.setBounds(inner.removeFromTop(30));
        inner.removeFromTop(10);
        p.outputLabel.setBounds(inner.removeFromTop(18));
        p.outputEditor.setBounds(inner.removeFromTop(110));
        p.statsLabel.setBounds(inner.removeFromTop(34));
        p.decryptButton.setBounds(inner.removeFromTop(30));
        inner.removeFromTop(8);
        p.decryptedLabel.setBounds(inner.removeFromTop(18));
        p.decryptedEditor.setBounds(inner);
    };

    layoutPanel(substitutionPanel, left);
    layoutPanel(transpositionPanel, middle);
    layoutPanel(otpPanel, right);
}

void MainComponent::encryptAll()
{
    const auto rawPlaintext = toStd(plaintextEditor.getText());
    const auto normalized = SimpleSubstitution::normalize(rawPlaintext);

    if (normalized.empty())
    {
        updateStatus("Enter at least one alphabetic character (Aâ€“Z).", true);
        return;
    }

    try
    {
        const auto substitutionKey = toStd(substitutionPanel.keyEditor.getText());
        const auto transpositionKeys = toStd(transpositionPanel.keyEditor.getText());

        auto start = std::chrono::steady_clock::now();
        substitutionResult.ciphertext = substitution.encrypt(normalized, substitutionKey);
        auto end = std::chrono::steady_clock::now();
        substitutionResult.encryptionMicroseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        substitutionResult.roundTripPassed =
            substitution.decrypt(substitutionResult.ciphertext, substitutionKey) == normalized;

        const auto comma = transpositionKeys.find(',');
        if (comma == std::string::npos || transpositionKeys.find(',', comma + 1) != std::string::npos)
            throw std::invalid_argument("Double transposition needs exactly two comma-separated keys.");

        const auto key1 = transpositionKeys.substr(0, comma);
        const auto key2 = transpositionKeys.substr(comma + 1);
        start = std::chrono::steady_clock::now();
        transpositionResult.ciphertext = transposition.encrypt(normalized, key1, key2);
        end = std::chrono::steady_clock::now();
        transpositionResult.encryptionMicroseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        transpositionResult.roundTripPassed =
            transposition.decrypt(transpositionResult.ciphertext, key1, key2) == normalized;

        start = std::chrono::steady_clock::now();
        otpResult.key = otp.generateKey(normalized.size());
        otpResult.ciphertext = otp.encrypt(normalized, otpResult.key);
        end = std::chrono::steady_clock::now();
        otpResult.encryptionMicroseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        otpResult.roundTripPassed = otp.decrypt(otpResult.ciphertext, otpResult.key) == normalized;

        substitutionPanel.statsLabel.setText(statText(substitutionResult), juce::dontSendNotification);
        transpositionPanel.statsLabel.setText(statText(transpositionResult), juce::dontSendNotification);
        otpPanel.statsLabel.setText(statText(otpResult), juce::dontSendNotification);
        otpPanel.keyEditor.setText(toJuice(otpResult.key), juce::dontSendNotification);
        substitutionPanel.decryptedEditor.clear();
        transpositionPanel.decryptedEditor.clear();
        otpPanel.decryptedEditor.clear();

        updateStatus("Encrypted " + juce::String(static_cast<juce::int64>(normalized.size()))
                      + " normalized letters. OTP key is freshly generated; keep it secret and never reuse it.");
    }
    catch (const std::exception& e)
    {
        updateStatus(juce::String("Input/key error: ") + e.what(), true);
    }
}

void MainComponent::decryptPanel(int panelIndex)
{
    try
    {
        std::string decrypted;
        if (panelIndex == 0)
        {
            decrypted = substitution.decrypt(substitutionResult.ciphertext,
                                              toStd(substitutionPanel.keyEditor.getText()));
            substitutionPanel.decryptedEditor.setText(toJuice(decrypted), juce::dontSendNotification);
        }
        else if (panelIndex == 1)
        {
            const auto keys = toStd(transpositionPanel.keyEditor.getText());
            const auto comma = keys.find(',');
            if (comma == std::string::npos || keys.find(',', comma + 1) != std::string::npos)
                throw std::invalid_argument("Use exactly two comma-separated transposition keys.");
            decrypted = transposition.decrypt(transpositionResult.ciphertext,
                                              keys.substr(0, comma), keys.substr(comma + 1));
            transpositionPanel.decryptedEditor.setText(toJuice(decrypted), juce::dontSendNotification);
        }
        else
        {
            decrypted = otp.decrypt(otpResult.ciphertext, otpResult.key);
            otpPanel.decryptedEditor.setText(toJuice(decrypted), juce::dontSendNotification);
        }

        const auto original = SimpleSubstitution::normalize(toStd(plaintextEditor.getText()));
        updateStatus(decrypted == original ? "Decryption successful: recovered plaintext matches normalized input."
                                           : "Decryption completed, but the result does not match the current input.",
                     decrypted != original);
    }
    catch (const std::exception& e)
    {
        updateStatus(juce::String("Decryption error: ") + e.what(), true);
    }
}

void MainComponent::updateStatus(const juce::String& message, bool isError)
{
    statusLabel.setText(message, juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId,
                          isError ? juce::Colour(255, 132, 132) : juce::Colour(151, 220, 171));
}

juce::String MainComponent::toJuice(const std::string& value)
{
    return juce::String(value.c_str());
}

std::string MainComponent::toStd(const juce::String& value)
{
    return value.toStdString();
}


