#ifndef STATE_H_
#define STATE_H_

#include <QObject>

#include <string>
#include <string_view>

class State : public QObject {
    Q_OBJECT

public:
    class Item {
    private:
        std::string url_;

    public:
        Item(std::string url) : url_(url) {}
        ~Item() {}

        std::string_view url() const { return url_; }
    };

private:
    std::vector<Item> items_;
    int current_;
    bool autoplay_;

public:
    State()
        : autoplay_(false)
        , current_(0)
    {
    }

    ~State() override {}

    void addItem(Item item)
    {
        items_.push_back(item);
        changed();
    }

    template<typename... Args>
    void add(Args && ...args)
    {
        items_.emplace_back(args...);
        changed();
    }

    bool autoplay() const { return autoplay_; }
    int currentIndex() const { return current_; }

    const Item& current() const { return items_[current_]; }
    const std::vector<Item>& items() const { return items_; }

public slots:
    void setAutoplay(bool autoplay)
    {
        autoplay_ = autoplay;
        changed();
    }

    void setCurrentIndex(int index)
    {
        current_ = index >= 0 && index < items_.size() ? index : 0;
        changed();
    }

    void next()
    {
        current_ = (current_ + 1) % items_.size();
        changed();
    }

    void prev()
    {
        current_ = (current_ -  1 + items_.size()) % items_.size();
        changed();
    }

signals:
    void changed();
};

#endif