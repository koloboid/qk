#pragma once

#include <QMap>
#include <QVariant>
#include <QExplicitlySharedDataPointer>
#include <QSqlRecord>
#include <qk/core/enum.hpp>
#include <qk/core/error.hpp>

namespace Qk {
namespace Db {

class IField;
class ITable;
class Driver;

using namespace Qk::Core;

class RowState : public Qk::Core::Enum
{
    Q_OBJECT

public:
    enum Value
    {
        New,
        Original,
        Modified,
        Invalid
    };
    QKENUM(Value)
};

class IRow
{
public:
    IRow();
    IRow(const ITable& pTable, RowState::Value pState = RowState::New);
    virtual ~IRow();

public:
    void reset();
    virtual QList<Error> validate() const;
    virtual void reload();
    virtual void save(Driver* pDrv = 0);
    virtual void drop(Driver* pDrv = 0);
    virtual void load(const QSqlRecord& pRecord);

    QVariant get(const IField& pField) const { return get(&pField); }
    IRow& set(const IField& pField, const QVariant& pValue) { return set(&pField, pValue); }
    template<class T>
    T get(const IField& pField) const { return get<T>(&pField); }
    template<class T>
    IRow& set(const IField& pField, const T& pValue) { return set<T>(&pField, pValue); }

    QVariant get(const IField* pField) const { return mData->mValues.value(pField); }
    IRow& set(const IField* pField, const QVariant& pValue);
    template<class T>
    T get(const IField* pField) const { return QVariant::value<T>(mData->mValues.value(pField)); }
    template<class T>
    IRow& set(const IField* pField, const T& pValue) { return set(pField, QVariant::fromValue<T>(pValue)); }

    QVariant primaryValue() const;
    const ITable* table() const { return mData->mTable; }
    bool operator!() const { return state() == RowState::Invalid || mData->mTable == nullptr; }
    RowState::Value state() const { return mData->mState; }

protected:
    IRow& setPrivate(const IField* pField, const QVariant& pValue);

private:
    class Data : public QSharedData
    {
    public:
        const ITable* mTable;
        QMap<const IField*, QVariant> mValues;
        RowState::Value mState;

    public:
        Data(const ITable& pTable, RowState::Value pState)
            : mTable(&pTable), mState(pState)
        {
        }
        Data()
            : mTable(0), mState(RowState::Invalid)
        {
        }
    };
    QExplicitlySharedDataPointer<Data> mData;
};

template<class TTable>
class Row : public IRow
{
public:
    typedef TTable TableType;

public:
    Row()
    {
    }

    explicit Row(const IRow& pRow)
        : IRow(pRow)
    {
    }

    Row(const TTable& pTable)
        : IRow(pTable)
    {
    }

    Row(const TTable* pTable)
        : IRow(*pTable)
    {
    }

public:
    template<class TField>
    typename TField::Type get(const TField& pField) const
    {
        static_assert(std::is_same<typename TField::Table, TTable>::value, "Type of model for this field must be the same as row's model");
        QVariant v = IRow::get(pField);
        return v.value<typename TField::Type>();
    }

    template<class TField>
    typename TTable::Row& set(const TField& pField, typename TField::Type pValue)
    {
        static_assert(std::is_same<typename TField::Table, TTable>::value, "Type of model for this field must be the same as row's model");
        static_assert(!TField::ReadOnly, "This field is read-only. You can change this field only by calling Row::setPrivate function");
        IRow::set(pField, QVariant::fromValue<typename TField::Type>(pValue));
        return static_cast<typename TTable::Row&>(*this);
    }

    template<class TField>
    typename TTable::Row& operator()(const TField& pField, typename TField::Type pValue)
    {
        return set(pField, pValue);
    }
    template<class TField>
    typename TField::Type operator()(const TField& pField) const
    {
        return get(pField);
    }

    const TTable* table() const { return static_cast<const TTable*>(IRow::table()); }

protected:
    template<class TField>
    typename TTable::Row& setPrivate(const TField& pField, typename TField::Type pValue)
    {
        static_assert(std::is_same<typename TField::Table, TTable>::value, "Type of model for this field must be the same as row's model");
        IRow::setPrivate(&pField, QVariant::fromValue<typename TField::Type>(pValue));
        return static_cast<typename TTable::Row&>(*this);
    }
};

}
}

#define QKROWCTOR(pRowClass, pBaseClass) public: \
    pRowClass() : pBaseClass() { } \
    explicit pRowClass(const IRow& pRow) : pBaseClass(pRow) { } \
    pRowClass(const typename pBaseClass::TableType& pTable) : pBaseClass(pTable) { } \
    pRowClass(const typename pBaseClass::TableType* pTable) : pBaseClass(pTable) { };
