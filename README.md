# PSoC Auto Pilot

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
`I2C_I2C_IRQ < ISR_MAIN = ISR_SENSOR < その他`
