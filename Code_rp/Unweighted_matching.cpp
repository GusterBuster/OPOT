#include "bits/stdc++.h"

template <class T, class U>
inline bool smin(T &x, const U &y) {
  return y < x ? x = y, 1 : 0;
}
template <class T, class U>
inline bool smax(T &x, const U &y) {
  return x < y ? x = y, 1 : 0;
}

using LL = long long;
using PII = std::pair<int, int>;

namespace IO {
constexpr bool UNSAFE = false;
constexpr int GLOB_BUF_SIZE = 1 << 15;
#ifndef DEBUG
#define CHANGE_DEFAULT_STREAMS
static struct FastInput {
  FastInput() {
    if constexpr (UNSAFE) {
      chars_read = fread(buf, 1, BUF_SIZE, in);
      buf_pos = 0;
      buf[0] = (chars_read == 0 ? -1 : buf[0]);
    }
  }
  static constexpr int BUF_SIZE = GLOB_BUF_SIZE;
  char buf[BUF_SIZE];
  size_t chars_read = 0;
  size_t buf_pos = 0;
  FILE* in = stdin;
  char cur = 0;
  inline char get_char() {
    if constexpr (!UNSAFE) {
      if (buf_pos >= chars_read) {
        chars_read = fread(buf, 1, BUF_SIZE, in);
        buf_pos = 0;
        buf[0] = (chars_read == 0 ? -1 : buf[0]);
      }
    }
    return cur = buf[buf_pos++];
  }
  template <typename T>
  inline FastInput* tie(T) {
    return this;
  }
  inline void sync_with_stdio(bool) {}
  inline explicit operator bool() { return cur != -1; }
  inline static bool is_blank(char c) { return c <= ' '; }
  inline bool skip_blanks() {
    while (is_blank(cur) && cur != -1) get_char();
    return cur != -1;
  }
  inline FastInput& operator>>(char& c) {
    skip_blanks();
    c = cur;
    return *this;
  }
  inline FastInput& operator>>(std::string& s) {
    if (skip_blanks()) {
      s.clear();
      do {
        s += cur;
      } while (!is_blank(get_char()));
    }
    return *this;
  }
  template <typename T>
  inline FastInput& read_integer(T& n) {
    // unsafe, doesn't check that characters are actually digits
    n = 0;
    if (skip_blanks()) {
      int sign = +1;
      if (cur == '-') {
        sign = -1;
        get_char();
      }
      do {
        n += n + (n << 3) + cur - '0';
      } while (!is_blank(get_char()));
      n *= sign;
    }
    return *this;
  }
  template <typename T>
  inline typename std::enable_if<std::is_integral<T>::value, FastInput&>::type
  operator>>(T& n) {
    return read_integer(n);
  }
#if !defined(_WIN32) || defined(_WIN64)
  inline FastInput& operator>>(__int128& n) { return read_integer(n); }
#endif
  template <typename T>
  inline typename std::enable_if<std::is_floating_point<T>::value,
                                 FastInput&>::type
  operator>>(T& n) {
    // not sure if really fast, for compatibility only
    n = 0;
    if (skip_blanks()) {
      std::string s;
      (*this) >> s;
      sscanf(s.c_str(), "%lf", &n);
    }
    return *this;
  }
} fast_input;
#define cin IO::fast_input
static struct FastOutput {
  static constexpr int BUF_SIZE = GLOB_BUF_SIZE;
  char buf[BUF_SIZE];
  size_t buf_pos = 0;
  static constexpr int TMP_SIZE = GLOB_BUF_SIZE;
  char tmp[TMP_SIZE];
  FILE* out = stdout;
  inline void put_char(char c) {
    buf[buf_pos++] = c;
    if (buf_pos == BUF_SIZE) {
      fwrite(buf, 1, buf_pos, out);
      buf_pos = 0;
    }
  }
  ~FastOutput() { fwrite(buf, 1, buf_pos, out); }
  inline FastOutput& operator<<(char c) {
    put_char(c);
    return *this;
  }
  inline FastOutput& operator<<(const char* s) {
    while (*s) put_char(*s++);
    return *this;
  }
  inline FastOutput& operator<<(const std::string& s) {
    for (auto x : s) put_char(x);
    return *this;
  }
  template <typename T>
  inline char* integer_to_string(T n) {
    // beware of TMP_SIZE
    char* p = tmp + TMP_SIZE - 1;
    if (n == 0)
      *--p = '0';
    else {
      bool is_negative = false;
      if (n < 0) {
        is_negative = true;
        n = -n;
      }
      while (n > 0) {
        *--p = (char)('0' + n % 10);
        n /= 10;
      }
      if (is_negative) *--p = '-';
    }
    return p;
  }
  template <typename T>
  inline typename std::enable_if<std::is_integral<T>::value, char*>::type
  stringify(T n) {
    return integer_to_string(n);
  }
#if !defined(_WIN32) || defined(_WIN64)
  inline char* stringify(__int128 n) { return integer_to_string(n); }
#endif
  template <typename T>
  inline typename std::enable_if<std::is_floating_point<T>::value, char*>::type
  stringify(T n) {
    sprintf(tmp, "%.17f", n);
    return tmp;
  }
  template <typename T>
  inline FastOutput& operator<<(const T& n) {
    auto p = stringify(n);
    for (; *p != 0; p++) put_char(*p);
    return *this;
  }
} fast_output;
#define cout IO::fast_output
#endif
}  // namespace IO
struct HopcroftKarp {
  std::vector<int> g, l, r;
  int ans;
  HopcroftKarp(int n, int m, const std::vector<PII> &e)
      : g(e.size()), l(n, -1), r(m, -1), ans(0) {
    std::vector<int> deg(n + 1);
    for (auto &[x, y] : e) deg[x]++;
    for (int i = 1; i <= n; i++) deg[i] += deg[i - 1];
    for (auto &[x, y] : e) g[--deg[x]] = y;

    std::vector<int> a, p, q(n);
    for (;;) {
      a.assign(n, -1), p.assign(n, -1);
      int t = 0;
      for (int i = 0; i < n; i++)
        if (l[i] == -1) q[t++] = a[i] = p[i] = i;

      bool match = false;
      for (int i = 0; i < t; i++) {
        int x = q[i];
        if (~l[a[x]]) continue;
        for (int j = deg[x]; j < deg[x + 1]; j++) {
          int y = g[j];
          if (r[y] == -1) {
            while (~y) r[y] = x, std::swap(l[x], y), x = p[x];
            match = true, ans++;
            break;
          }

          if (p[r[y]] == -1)
            q[t++] = y = r[y], p[y] = x, a[y] = a[x];
        }
      }

      if (!match) break;
    }
  }
};

void solve() {
  int l, r, m;
  cin >> l >> r >> m;
  std::vector<PII> e(m);
  for (auto &[x, y] : e) cin >> x >> y;

  HopcroftKarp hk(l, r, e);
  cout << hk.ans << "\n";

  for (int i = 0; i < l; i++) {
    if (~hk.l[i]) {
      cout << i << " " << hk.l[i] << "\n";
    }
  }
}

int main() {
  // freopen("t.in", "r", stdin);
  // freopen("t.out", "w", stdout);
  
  std::ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int t = 1;
  
  // cin >> t;
  
  while (t--) {
    solve();
  }
  return 0;
}
