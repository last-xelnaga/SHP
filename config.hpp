
#ifndef CONFIG_HPP
#define CONFIG_HPP

class Config
{
    static Config* p_instance;

    char p_network_address[32];
    unsigned int network_port;

    Config(void);

  public:
    int read_config(const char* p_cfg_filename);

    char* read_network_address(void);

    unsigned int read_network_port(void);

    static Config* instance(
        void)
    {
        if (!p_instance)
            p_instance = new Config;
        return p_instance;
    }
};

#endif // CONFIG_HPP
