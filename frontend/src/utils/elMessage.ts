import { ElMessage } from 'element-plus'
import pubSubService from './pubSubService'

pubSubService.subscribe('requestSuccess', (message: string) => {
  ElMessage.success(message)
})

pubSubService.subscribe('requestFailed', (error: string) => {
  console.error(error)
  ElMessage.error(error)
})

pubSubService.subscribe('notLoginOrTokenExpired', (error: string) => {
  console.error(error)
  ElMessage.error(error)
})