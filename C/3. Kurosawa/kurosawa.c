#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacob Niemeir");
MODULE_DESCRIPTION("Output an Akira Kurosawa quote to kernel logs");
MODULE_VERSION("1.00");

// It is good practice to declare variables that are outside of functions in
// kernel modules as static to avoid name collisions
static char quotes[9][206] = {
    "I am not a special person, I am not especially strong; I am not "
    "especially gifted. I simply do not like to show my weakness, and I hate "
    "to lose, so I am a person who tries hard. That's all there is to me.",
    "In a mad world, only the mad are sane.",
    "If you look at everything straight on, there is nothing to be afraid of",
    "The best way to protect your place in this world is to do nothing at all",
    "Human beings are unable to be honest with themselves about themselves. "
    "They cannot talk about themselves without embellishing.",
    "When you hire someone cheaply, you pay a high price",
    "The shadow of a man can never stand up and walk on its own.",
    "Misfortune teaches us the truth",
    "Man just wants to forget the bad stuff and believe in the made-up good "
    "stuff"};

// A kernel module's init function
// It is also advised to declare kernel module functions as static for the
// reason mentioned above
static int __init kurosawa_init(void) {
  // We use an unsigned integer to ensure that get_random_bytes returns a
  // positive value, as array indices can only be positive
  unsigned int random_index;
  // get_random_bytes is the function used to call the Linux RNG
  get_random_bytes(&random_index, sizeof(random_index));
  // We constrain the generated number to the number of elements in the array
  // ARRAY_SIZE is a macro in the kernel headers defined as
  // sizeof(array) / sizeof(array[0])
  random_index %= ARRAY_SIZE(quotes);
  // We print the element of quotes at index random_index to the kernel ring
  // buffer.
  // KERN_INFO is the log level, which indicates how important
  // the message is.
  printk(KERN_INFO "Akira Kurosawa Said: %s\n", quotes[random_index]);
  return 0;
}

// A kernel module's cleanup function undoes the actions performed by the module
// Since this module doesn't change anything in the kernel, no cleanup is
// necessary
static void __exit kurosawa_cleanup(void) {
  printk(KERN_INFO "Life is brief...\n");
}

module_init(kurosawa_init);
module_exit(kurosawa_cleanup);
