
Practica 2 

Por: Ricardo Jimenez Sanchez
     Alexander Sinsel Ayala

Sistemas embebidos II

Para esta práctica se buscaba implementar una comunicación entre el acelerómetro BMI160 y un script de Python el cual esta encargado de mostrar gráficamente los movimientos en el sensor. Para esto se requieren de diferentes drivers e implementaciones empezando por la de I2C no bloqueante para la colaboración de tareas. Para este driver nos basamos en el de UART implementado por el profesor, en el cual tuvimos que agregar unas opciones para maestro y esclavo y usar las librerías de fsl_i2c del SDK.

Seguido de esto decidimos implementar las librerías del sensor BMI160 para las cuales se tuvo que ver detenidamente el data sheet y de ahi obtener valores de configuración y direcciones necesarias para la misma, una vez con los datos correctos se procedió a utilizar la librería de I2C implementada para configurar y leer los datos de estos sensores, estos se guardan en una estructura definida.

Seguido de esto se implementó una etapa de calibración del sensor la cual consta de tomar 100 muestras iniciales con el sensor inmóvil y obtener un promedio de estas, así sabemos que tanto se mueve estando inmóvil y lo utilizamos para restar ese factor a los datos recibidos posteriormente. 

Una vez que se comprobó que los datos son recibidos de manera correcta imprimiéndolos en consola se agregaron los códigos dados para la conversion a roll, pitch y jaw y de ahi determinar la rotación del cubo, esto con los códigos proporcionados de Mahony en donde se tuvo que hacer uso de una función sencilla MahonyAHRSupdateIMU.

Por último, para mandar estos datos por el puerto USB y leerlos por el script de python se  hizo uso de un ejemplo del SDK en el cual se agrego todo el código y se hizo uso de una de las funciones USB_DeviceCdcAcmSend con la cual se podía enviar la estructura con los valores para solo especificar el puerto en el script y visualizar el movimiento gráficamente. 

