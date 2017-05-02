#ifndef CORE_CLASS_HPP
#define CORE_CLASS_HPP

class core_class
{
private:
    void notify_all_clients (
        void);

public:
    void register_probe (
        void);
    void register_client (
        void);

    void process_command (
        void);
    void process_event (
        void);
};

#endif // CORE_CLASS_HPP
