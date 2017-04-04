#include "GUI.h"

GUI::GUI()
{
    setEnabled();
}

void GUI::setup(const vector<string> &toggleNames)
{
    int y = 50;
    int x = 50;
    int width = 500;

    _panel.setDefaultWidth(width);
    _panel.setup("KNN Features", "panel0.xml", x, y);

    int count = 0;
    int numPerPanel = 100;
    for (int i = 0; i < toggleNames.size(); i++)
    {
        ofxToggle * tog = new ofxToggle();
        _toggles.push_back(tog);
        string name = toggleNames[i];

        if (i >= 1 && i <= 128)
        {
            name = "Melodic_Interval_Histogram";
            i = 128; // skip
        }
        else if (i >= 140 && i <= 267)
        {
            name = "Basic_Pitch_Histogram";
            i = 267; // skip
        }
        else if (i >= 277 && i <= 288)
        {
            name = "Fifths_Pitch_Histogram";
            i = 288; // skip
        }

        _panel.add(tog->setup(name, true));
        count++;
    }

    _featureMask.resize(toggleNames.size(), true);
    _updateFeatureMask();
}

void GUI::update()
{
    if (isEnabled()) _updateFeatureMask();
}

void GUI::draw()
{
    if (isEnabled())
    {
        _panel.draw();
    }
}

void GUI::setEnabled(bool enabled)
{
    _enabled = enabled;
    if (_enabled)
    {
        _panel.registerMouseEvents();
    }
    else
    {
        _panel.unregisterMouseEvents();
    }
}

void GUI::toggleEnabled()
{
    setEnabled(!isEnabled());
}

// print index of enabled features
void GUI::printFeatureMask()
{
    cout << "FEATURE MASK:" << endl;
    for (int i = 0; i < _featureMask.size(); i++)
    {
        if (_featureMask[i]) cout << i << endl;
    }
}

const vector<bool> GUI::getFeatureMask()
{
    return _featureMask;
}

bool GUI::isEnabled()
{
    return _enabled;
}

inline void setMaskValues(vector<bool>& mask, int start, int end, bool val)
{
    for (; start <= end; start++)
    {
        mask[start] = val;
    }
}

// gross as fuck. Sorry to whoever reads this...
int GUI::_featureIndex2ToggleIndex(int featIndex)
{
    if (featIndex == 0) return featIndex;
    if (featIndex >= 1 && featIndex <= 128)
    {
        // "Melodic_Interval_Histogram"
       return 1;
    }
    else if (featIndex >= 129 && featIndex <= 139)
    {
        return featIndex - 127;
    }
    else if (featIndex >= 140 && featIndex <= 267)
    {
        // "Basic_Pitch_Histogram"
        return 13;
    }
    else if (featIndex >= 268 && featIndex <= 276)
    {
        return featIndex - 127 * 2;
    }
    else if (featIndex >= 277 && featIndex <= 288)
    {
        // "Fifths_Pitch_Histogram"
        return 23;
    }
    else
    {
        return featIndex - (127 * 2) - 11;
    }
}

void GUI::_updateFeatureMask()
{
    for (int i = 0; i < _featureMask.size(); i++)
    {
        bool val = false;
        if (*_toggles[_featureIndex2ToggleIndex(i)]) val = true;

        if (i >= 1 && i <= 128)
        {
            // "Melodic_Interval_Histogram"
            setMaskValues(_featureMask, 1, 128, val);
            i = 128; // skip
        }
        else if (i >= 140 && i <= 267)
        {
            // "Basic_Pitch_Histogram"
            setMaskValues(_featureMask, 140, 267, val);
            i = 267;
        }
        else if (i >= 277 && i <= 288)
        {
            // "Fifths_Pitch_Histogram"
            setMaskValues(_featureMask, 277, 288, val);
            i = 288;
        }
        else
        {
            _featureMask[i] = val;
        }
    }
}
