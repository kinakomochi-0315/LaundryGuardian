# LaundryGuardian

## What is this?
気圧を測定して、雨が降りそうなときに画面と音で警告するシステムです。

## 動作環境
- M5StickC Plus2
- M5 EnvUnit Ⅳ (BMP280)
- PlatformIO

## インストール
1. このリポジトリをクローンなりなんなりして、手元に持ってきます。
2. PlatformIOをインストールしたVSCodeで開きます。
3. M5StickC Plus2をUSBで接続します。
4. `src/`に`secrets.h`を作成して、以下の内容を追加してください。
```c++
#ifndef SECRETS_H
#define SECRETS_H

auto WIFI_SSID = "YOUR-SSID";
auto WIFI_PSWD = "YOUR-PASSWORD";
auto LINE_TOKEN = "LINE Message APIの長期トークン";

#endif //SECRETS_H
```
5. 書き込みます。
6. Let's enjoy!

## 使い方
- デフォルトでは、画面に気圧が表示されています。
- 気圧が**1000hPa**を下回ると警告画面が表示されるとともに警告音が鳴ります。LINE通知も送信されます。
- 警告画面が表示されているときにAボタン(画面横のボタン)を押すと警告がストップします。