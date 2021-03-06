# PSoC Auto Pilot

## 開発環境
* PSoC Creator  3.0
	* Windows 7 Professional
	* Core i5 M460@2.53GHz
* [CY8C3866PVI-021](http://japan.cypress.com/?mpn=CY8C3866PVI-021)(本体)
* [MPU-9150](https://strawberry-linux.com/catalog/items?code=12150)(9軸センサー)
	* 簡易データシート&取説(https://strawberry-linux.com/pub/mpu-9150-manual.pdf)
	* レジスタ・リファレンスマニュアル(https://strawberry-linux.com/pub/RM-MPU-9150A-00.pdf)

## 設定方法
### PSoC Creatorのインストール
[トラ技の本](http://shop.cqpub.co.jp/hanbai/books/48/48221.html)の第１章参照。
### MiniProg3の設定
#### ドライバのインストール(Windows8のみ)
以下の__Answer:__に書いてある手順に従う  
http://japan.cypress.com/?app=forum&id=2232&rID=74548&source=shop
#### プログラムの書き込み
Debug > Program などで「Select Debug Target」みたいなダイアログが出てきたら、MiniProg3を選択して「Port Setting」から以下のように設定する。

* Active Protocol : SWD
* Clock Speed : 1.6MHz
* Power : 5.0V
* Acquire Mode : Reset
* Connector : 10 pin

### USBUARTのドライバの設定(Windows8のみ)
http://www.besttechnology.co.jp/modules/d3blog/details.php?bid=119

## 注意
### 初期化の順番
	void init(){
		CyGlobalIntEnable;
		/*各種コンポーネントの初期化*/
		PWM_Start();
		/*最後に割り込みスタート*/
		ISR_MAIN_StartEx(ISR_MAIN);
	}
### 割り込み優先順位(Priorityの値順)
`I2C_I2C_IRQ < UART関係 < ISR_SENSOR <= ISR_MAIN < その他`