# PSoC Auto Pilot

## 開発環境
* PSoC Creator  3.0
	* Windows 7 Professional
	* Core i5 M460@2.53GHz
* [CY8C3866PVI-021](http://japan.cypress.com/?mpn=CY8C3866PVI-021)(本体)
* [MPU-9150](https://strawberry-linux.com/catalog/items?code=12150)(9軸センサー)
	* 簡易データシート&取説(https://strawberry-linux.com/pub/mpu-9150-manual.pdf)
	* レジスタ・リファレンスマニュアル(https://strawberry-linux.com/pub/RM-MPU-9150A-00.pdf)

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