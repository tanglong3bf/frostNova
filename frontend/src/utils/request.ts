import axios, { type AxiosResponse } from 'axios'
import pubSubService from '@/utils/pubSubService'
import { useToken } from '@/composables/token'
import './types/request.d.ts'

const instance = axios.create({
  baseURL: 'http://localhost:8000',
  timeout: 5000
})

instance.interceptors.request.use(
  config => {
    const token = useToken()
    if (token.value !== '') {
      // 每次请求时添加 token
      config.headers['Authorization'] = `Bearer ${token.value}`
    }
    return config
  },
  error => {
    pubSubService.publish('requestFailed', error || 'Error')
    return Promise.reject(error)
  }
)

instance.interceptors.response.use(
  (response: AxiosResponse<NullableResp>):any => {
    if (response.status === 204) {
      return undefined
    }
    const { code, message, error, data } = response.data || {}
    // 如果 code 小于 0，则表示发生了错误
    if (code !== undefined && code < 0) {
      pubSubService.publish('requestFailed', error || 'Error')
      return Promise.reject(new Error(error || 'Error'))
    } else if (message !== undefined) {
      pubSubService.publish('requestSuccess', message)
    }
    return data
  },
  error => {
    let message = 'Error'
    if (error.response.data.error) {
      message = error.response.data.error
    }
    else if (error.status === 403) {
      message = '权限不足'
    }
    else if (error.status === 401) {
      message = "请您先登录"
      pubSubService.publish('notLoginOrTokenExpired', message)
      return Promise.reject(error)
    }
    pubSubService.publish('requestFailed', message)
    return Promise.reject(error)
  }
)

export default instance
