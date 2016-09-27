import org.apache.tools.ant.taskdefs.condition.Os

class KeyTool {
	String keyTool

	KeyTool() {
		keyTool = System.getProperty('sun.boot.library.path')

		if (keyTool == null) {
			keyTool = System.getProperty('java.home') + File.separator + 'bin'
		}

		keyTool += File.separator
		keyTool += 'keytool'
		keyTool += (Os.isFamily(Os.FAMILY_WINDOWS)) ? '.exe' : ''
	}

	@Override
	String toString() {
		keyTool
	}
}
