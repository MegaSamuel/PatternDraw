#ifndef UNI_STATIC_H
#define UNI_STATIC_H

//------------------------------------------------------------------------------

template<class TType>
class  TStatic
{
private:

    static TType  ms_tObject;

private:

    TStatic(const TStatic<TType>&);
    TStatic<TType>& operator = (const TStatic<TType>&);

protected:

    TStatic(void) {}

    virtual ~TStatic(void) {}

public:

    static  TType&  get_instance(void)
    {return  ms_tObject;}

};

template<typename TType>
TType  TStatic<TType>::ms_tObject;

//------------------------------------------------------------------------------

#endif // UNI_STATIC_H
