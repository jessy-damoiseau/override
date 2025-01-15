#include <unistd.h>
#include <stdio.h>


char user[256];

int verify_user_name()
{
  puts("verifying username....\n");
  return memcmp(user, "dat_wil", 7) != 0;
}

int verify_user_pass(const void *a1)
{
  return memcmp(a1, "admin", 5) != 0;
}

int main(int argc, const char **argv, const char **envp)
{
  char s[64];
  int v5;

  memset(s, 0, sizeof(s));
  v5 = 0;
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(user, 256, stdin);
  v5 = verify_user_name();
  if ( v5 )
  {
    puts("nope, incorrect username...\n");
  }
  else
  {
    puts("Enter Password: ");
    fgets(s, 100, stdin);
    v5 = verify_user_pass(s);
    puts("nope, incorrect password...\n");
  }
  return 1;
}