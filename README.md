# Repo for khronokernel.github.io

Built with [Jekyll](https://jekyllrb.com) and [minima](https://github.com/jekyll/minima) theme.

* [Site link](https://khronokernel.github.io)


## Building

```sh
# Install dependancies
bundle install
# Server site
bundle exec jekyll serve
```

If `LoadError: cannot load such file -- google/protobuf_c` is encountered, run the following:

```sh
sudo gem uninstall google-protobuf
sudo gem install google-protobuf -v 3.23.4 --platform=ruby
```

* Replace `3.23.4` with the version specified in the error message.