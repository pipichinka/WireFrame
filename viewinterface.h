#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H


class ViewInterface
{
public:
    ViewInterface() = default;
    virtual void onModelChange() = 0;

};

#endif // VIEWINTERFACE_H
