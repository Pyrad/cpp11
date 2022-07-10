# cpp11
A repository for testing various features in CPP (c98, c11, and so on)



## Note: !!! Always Keeping 2 Repositories Same !!!

There are 2 repositories working as backups for each other, which are

- From **Gitee**: [git@gitee.com:pyrad/cpp11.git]( git@gitee.com:pyrad/cpp11.git)
- From **GitHub**: [git@github.com:Pyrad/cpp11.git](git@github.com:Pyrad/cpp11.git)

So the 2 repositories must be ***exactly the same*** as each other.

For daily use, clone it from Gitee, and add another URL from Github as `push` URL to keep them synced.

```bash
### Clone to local folder
$ git clone git@gitee.com:pyrad/cpp11.git
### Check current fetch & push origin
$ git remote -v 
origin  git@gitee.com:pyrad/cpp11.git (fetch)
origin  git@gitee.com:pyrad/cpp11.git (push)
### Add another repo as *PUSH* origin
$ git remote set-url --add origin git@github.com:Pyrad/cpp11.git
### Check current fetch & push origin again
$ git remote -v
origin  git@gitee.com:pyrad/cpp11.git (fetch)
origin  git@gitee.com:pyrad/cpp11.git (push)
origin  git@github.com:Pyrad/cpp11.git (push)
```



## How to build?

- Create a directory named `build` in current folder, then change directory to `build` just created

- Run CMake to generate Makefile

  ```bash
  $ cmake -G Ninja ../src -DCMAKE_BUILD_TYPE=Release
  ```

  

  If you want to generate `compile_commands.json` for editor's completion plugin, add option `-DCMAKE_EXPORT_COMPILE_COMMANDS=1`

  ```bash
  $ cmake -G Ninja ../src -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  ```

  More about `compile_commands.json`, refer to [**clangd**](https://clangd.llvm.org/installation)

  

- Make

  ```bash
  $ cmake --build .
  ```

  

- Since currently I set the target name as `mymainrun`, so after compilation, a binary named `mymainrun` is created.
