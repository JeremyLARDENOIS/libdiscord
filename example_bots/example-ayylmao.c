//
// Created by dxing97 on 11/11/17.
//

/*
 * This bot will respond to every "ayy" with "lmao", or any other trigger-response pair you want
 */

#include <libdiscord.h>

#include <getopt.h>
#include <signal.h>

static int signaled = 0;
static int try_recovery = 1;
unsigned long log_level = 31;

char *trigger = "ayy", *response = "lmao";

void int_handler(int i) {
    signaled = 1;
}

/*
 * main way of user interaction with libdiscord
 * the user callback returns 0 if everything is OK
 *
 * ld_context contains info about the bot. User code shouldn't have to mess with it.
 * ld_callback_reason is the reason for the library calling the callback. See the enum declaration in libdiscord.h
 * data and len contain data that may be needed for the callback, their use depends on the reason for the callback.
 */
int callback(struct ld_context *context, enum ld_callback_reason reason, void *data, int len) {
    // if content == "ayy", POST "lmao" to that channel
    return 0;
}

int main(int argc, char *argv[]) {

    /*
     * get input arguments
     * initialize context from arguments
     * enter event loop
     * in loop:
     *  update events, process callbacks until signal is raised
     * clean up
     */
    int c;
    char *bot_token = NULL;

    if(argc == 1) {
        goto HELP;
    }

    while(1) {
        //options: help, bot token
        //if bot token isn't specified, exit
        static struct option long_options[] = {
                {"bot-token",      required_argument, 0, 't'},
                {"help",           no_argument,       0, 'h'},
                {"log-level",      required_argument, 0, 'l'},
                {"game",           required_argument, 0, 'g'},
                {"trigger",        required_argument, 0, 'r'},
                {"response",       required_argument, 0, 'R'},
                {"abort-on-error", no_argument,       0, 'a'},
                {0, 0,                                0, 0}
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "ht:l:g:r:R:a", long_options, &option_index);

        if(c == -1) {
            break;
        }

        switch(c) {
            case 'h':
            HELP:
                printf("libdiscord example bot: ayylmao - basic call and response bot\n"
                       "%s [-t bot_token]\n\n"
                       "Options: \n\t"
                       "-t, --bot-token [bot_token]\n\t\t"
                       "Required. Discord bot token. See Discord developer pages on how to obtain one.\n\t"
                       "-r, --trigger [trigger_string]\n\t\t"
                       "Sets string that will trigger a response from the bot. Default is \"ayy\".\n\t"
                       "-R, --response [response_string]\n\t\t"
                       "Sets response that will be sent when the trigger is read. Default is \"lmao\".\n\t"
                       "-a --abort-on-error \n\t\t"
                       "If set, the bot will exit if the websocket connection is closed instead of trying to reconnect.\n\t"
                       "-h, --help\n\t\t"
                       "Displays this help dialog\n", argv[0]);
                return 0;
            case 't':
                bot_token = strdup(optarg);
                break;
            case 'l':
                log_level = strtoul(optarg, NULL, 10);
                break;
            case 'r':
                trigger = strdup(optarg);
                break;
            case 'R':
                response = strdup(optarg);
                break;
            case 'a':
                try_recovery = 1;
            default:
                abort();
        }
    }

    printf("Example bot 1 \"ayylmao\" starting up using libdiscord v%s\n", LD_VERSION);

    if(bot_token == NULL) {
        printf("Bot token not set! See example-ayylmao -h for details.");
        return 1;
    }
    printf("Initializing libdiscord with log level %lu\n", log_level);

    ld_init(log_level);

    signal(SIGINT, int_handler);
    lwsl_info("set response to %s", response);


    //initialize arguments to be passed into context initalization
    struct ld_context_options *info;
    info = calloc(1, sizeof(struct ld_context_options));
//    ld_init_context_info(info);

    info->bot_token = strdup(bot_token);
    info->user_callback = callback;

    free(bot_token);

    //initialize context with context info
    struct ld_context context;
    int ret = ld_context_init(info, &context);
    free(info);

    if(ret != LDE_OK){
        lwsl_err("error initializing libdiscord context, got %d", ret);
        return 1;
    }

//    handle = curl_easy_init();
    int count = 0;
    while(!signaled) {
        ret = ld_service(&context, 50); //service the connection
        if(ret != LDE_OK) {
            if(try_recovery == 1 && count < 4) {
                count++;
                lwsl_warn("ld_service returned (%d), retrying", ret);
            } else {
                signaled = 1;
                lwsl_notice("ld_service returned (%d), exiting", ret);
            }

        }
    }

    lwsl_info("disconnecting from discord");
    //destroy the context
    ld_context_destroy(&context);
//    curl_easy_cleanup(handle);


    return 0;
}
